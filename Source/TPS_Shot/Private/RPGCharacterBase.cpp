// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Ability/RPGAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	attributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	abilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	abilitySystemComponent->SetIsReplicated(true);
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return abilitySystemComponent;
}

void ARPGCharacterBase::InitializeAttribute()
{
	ApplyGameplayEffect(1, defaultAttributeEffect);
}

void ARPGCharacterBase::ApplyGameplayEffect(float level, TSubclassOf<UGameplayEffect> gameplayEffect)
{
	if (abilitySystemComponent)
	{
		// エフェクトの文脈情報
		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		// エフェクトのスケール値や適用対象の文脈が含まれる
		FGameplayEffectSpecHandle specHandle = abilitySystemComponent->MakeOutgoingSpec(gameplayEffect, level, effectContext);

		if (specHandle.IsValid())
		{
			// キャラクターに適用
			FActiveGameplayEffectHandle geHandle = abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		}
	}
}

void ARPGCharacterBase::GiveAbility()
{
	if (HasAuthority() && abilitySystemComponent)
	{
		for (auto ability : defaultAbilities)
		{
			abilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ability, 1, static_cast<int>(ability.GetDefaultObject()->abilityInputID), this));
		}
	}
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// abilitySystemComponentに自身のキャラクターを認識させる
	// サーバー側の処理
	abilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttribute();
	GiveAbility();
}

void ARPGCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	abilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttribute();

	if (abilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int>(EGASAbilityInputID::Confirm), static_cast<int>(EGASAbilityInputID::Cancel));
		abilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,Binds);
	}
}

void ARPGCharacterBase::HandleDamage(float damageAmount, const FHitResult& hitInfo,
                                     const struct FGameplayTagContainer& damageTags, ARPGCharacterBase* instigatorCharacter, AActor* damageCauser)
{
	// UKismetSystemLibrary::PrintString(this, TEXT("Damage"), true, true, FColor::Cyan);
}

void ARPGCharacterBase::HandleHealthChanged(float deltaValue, const struct FGameplayTagContainer& eventTags)
{
	// UKismetSystemLibrary::PrintString(this, TEXT("Health"), true, true, FColor::Cyan);

}

