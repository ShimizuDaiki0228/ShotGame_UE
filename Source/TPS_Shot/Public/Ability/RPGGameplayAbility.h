// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

enum class EGASAbilityInputID : uint8;
/**
 * 
 */
UCLASS()
class TPS_SHOT_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	URPGGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID abilityInputID = EGASAbilityInputID::None;
};
