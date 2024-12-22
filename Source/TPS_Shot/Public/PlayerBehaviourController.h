// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "PlayerBehaviourController.generated.h"

class ATPS_ShotCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_SHOT_API UPlayerBehaviourController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBehaviourController();

	void Initialized(ATPS_ShotCharacter* character);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

private:
	UPROPERTY()
	ATPS_ShotCharacter* _character;
};
