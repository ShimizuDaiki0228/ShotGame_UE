// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/RPGAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RPGCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "Ability/RPGAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


URPGAttributeSet::URPGAttributeSet()
	:Health(1.0f)
	,MaxHealth(1.0f)
	,Damage(0.0f)
{
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// GameplayEffectのコンテキスト情報（発動元やヒット結果など）を取得
	FGameplayEffectContextHandle context = Data.EffectSpec.GetContext();
	// アクションを行ったキャラクターのAbilitySystemComponent
	UAbilitySystemComponent* source = context.GetOriginalInstigatorAbilitySystemComponent();
	// GameplayEffectの発動元に関連付けられたタグ情報を取得するためのコード
	// 例えばDamageの中でもDamage.fireやDamage.iceなど識別することができる
	const FGameplayTagContainer& sourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float deltaValue = 0;
	// 属性に対してどのように値を変化させるかを確認
	// 現状は加算、乗算や上書き、除算もある
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// Magnitudeは属性に適用された変化量を表している
		deltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor* targetActor = nullptr;
	AController* targetController = nullptr;
	ARPGCharacterBase* targetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		targetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		targetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		targetCharacter = Cast<ARPGCharacterBase>(targetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Effectを発動したアクターを取得する
		AActor* sourceActor = nullptr;
		AController* sourceController = nullptr;
		ARPGCharacterBase* sourceCharacter = nullptr;
		if (source && source->AbilityActorInfo.IsValid() && source->AbilityActorInfo->AvatarActor.IsValid())
		{
			sourceActor = source->AbilityActorInfo->AvatarActor.Get();
			sourceController = source->AbilityActorInfo->PlayerController.Get();
			if (sourceController == nullptr && sourceActor != nullptr)
			{
				if (APawn* pawn = Cast<APawn>(sourceActor))
				{
					sourceController = pawn->GetController();
				}
			}

			if (sourceController)
			{
				sourceCharacter = Cast<ARPGCharacterBase>(sourceController->GetPawn());
			}
			else
			{
				sourceCharacter = Cast<ARPGCharacterBase>(sourceActor);
			}

			// GameplayEffectが発動した際の原因となるアクター
			// 例えば弾丸が命中した場合などは弾丸自体がEffectCauserになる
			if (context.GetEffectCauser())
			{
				sourceActor = context.GetEffectCauser();
			}
		}

		// ヒットに関する詳細（命中位置、法線、対象ベクトルなど）を取得
		FHitResult hitResult;
		if (context.GetHitResult())
		{
			hitResult = *context.GetHitResult();
		}

		const float localDamage = GetDamage();
		SetDamage(0.0f);

		if (localDamage > 0)
		{
			const float currentHealth = GetHealth();
			SetHealth(FMath::Clamp(currentHealth - localDamage, 0.0f, GetMaxHealth()));

			// ダメージを受けたときにどうするか
			if (targetCharacter)
			{
				targetCharacter->HandleDamage(localDamage, hitResult, sourceTags, sourceCharacter, sourceActor);

				targetCharacter->HandleHealthChanged(-localDamage, sourceTags);
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (targetCharacter)
		{
			targetCharacter->HandleHealthChanged(deltaValue, sourceTags);
		}
	}
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPGAttributeSet, Health);
	DOREPLIFETIME(URPGAttributeSet, Damage);
	DOREPLIFETIME(URPGAttributeSet, MaxHealth);
}

// 最大値が変更したときに現在の値を割合的に一致させる
void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& affectedAttribute,
	const FGameplayAttributeData& maxAttribute, float newMaxValue, const FGameplayAttribute& affectedAttributeProperty)
{
	UAbilitySystemComponent* abilityComp = GetOwningAbilitySystemComponent();
	const float currentMaxValue = maxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(currentMaxValue, newMaxValue) && abilityComp)
	{
		const float currentValue = affectedAttribute.GetCurrentValue();
		float newDelta = (currentMaxValue > 0.0f) ? (currentValue * newMaxValue / currentMaxValue) - currentValue : newMaxValue;

		abilityComp->ApplyModToAttributeUnsafe(affectedAttributeProperty, EGameplayModOp::Additive, newDelta);
	}
}

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& oldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, oldValue);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, oldValue);
}
