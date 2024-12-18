// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehaviourController.h"
#include "TPS_ShotCharacter.h"

// Sets default values for this component's properties
UPlayerBehaviourController::UPlayerBehaviourController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerBehaviourController::Initialized(ATPS_ShotCharacter* character)
{
	_character = character;
}

// Called when the game starts
void UPlayerBehaviourController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerBehaviourController::Move(const FInputActionValue& value)
{
	// input is a Vector2D
	FVector2D MovementVector = value.Get<FVector2D>();

	if (_character->Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = _character->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		_character->AddMovementInput(ForwardDirection, MovementVector.Y);
		_character->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UPlayerBehaviourController::Look(const FInputActionValue& value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (_character->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		_character->AddControllerYawInput(LookAxisVector.X);
		_character->AddControllerPitchInput(LookAxisVector.Y);
	}
}
