// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (_shooterCharacter == nullptr)
	{
		_shooterCharacter = Cast<ATPS_ShotCharacter>(TryGetPawnOwner());
	}
	if (_shooterCharacter)
	{
		FVector velocity = _shooterCharacter->GetVelocity();
		velocity.Z = 0;
		_playerMoveSpeed = velocity.Size();

		bIsInAir = _shooterCharacter->GetCharacterMovement()->IsFalling();

		bIsAccelerating = _shooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

		bAiming = _shooterCharacter->GetAiming();
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	_shooterCharacter = Cast<ATPS_ShotCharacter>(TryGetPawnOwner());
}
