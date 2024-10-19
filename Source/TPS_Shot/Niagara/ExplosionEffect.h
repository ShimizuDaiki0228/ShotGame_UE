// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraEffect.h"
#include "Components/SphereComponent.h"
#include "../TPS_ShotCharacter.h"
#include "ExplosionEffect.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AExplosionEffect : public ANiagaraEffect
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AExplosionEffect();

	void Initialized(ATPS_ShotCharacter* player);

private:
	USphereComponent* collisionSphere;

	void LauncherPlayer(ATPS_ShotCharacter* player);

	const float BLAST_RANGE = 250.0f;

	float CalculateBlastPowerRatio(ATPS_ShotCharacter* player);
	FVector CalculateLaunchVelocity(ATPS_ShotCharacter* player, float proximityValue);
};
