// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Ability/RPGAttributeSet.h"
#include "Ability/RPGGameplayAbility.h"
#include "RPGCharacterBase.generated.h"

class URPGAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class TPS_SHOT_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	float GetHealth() const {return attributeSet->GetHealth();}
	float GetMaxHealth() const {return attributeSet->GetMaxHealth();}

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitializeAttribute();
	void ApplyGameplayEffect(float level, TSubclassOf<UGameplayEffect> gameplayEffect);
	void GiveAbility();

	virtual void PossessedBy(AController* NewController) override;
	//クライアントとサーバーを同期させる目的
	// PlayerStateはサーバーで生成されて、クライアントにリプリケートされる
	// クライアントで abilitySystemComponent を正しく初期化するためには、PlayerStateがリプリケートされたタイミングで設定を再実行する必要がある
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> defaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> testAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<URPGGameplayAbility>> defaultAbilities;
public:
	// RPGAttributeSetから、対象のパラメータに変更があった時に呼ばれる
	void HandleDamage(float damageAmount, const FHitResult& hitInfo, const struct FGameplayTagContainer& damageTags, ARPGCharacterBase* instigatorCharacter, AActor* damageCauser);
	virtual void HandleHealthChanged(float deltaValue, const struct FGameplayTagContainer& eventTags);

protected:
	UPROPERTY()
	URPGAttributeSet* attributeSet;

	UPROPERTY()
	URPGAbilitySystemComponent* abilitySystemComponent;
};
