// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleSystemPoolActor.h"
#include "UPooledObjectBase.h"
#include "PooledParticleSystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UPooledParticleSystemComponent : public UParticleSystemComponent,
public UPooledObjectBase<AParticleSystemPoolActor>
{
	GENERATED_BODY()
	
public:
	UPooledParticleSystemComponent();

public:
	UPROPERTY()
	AParticleSystemPoolActor* Pool;

private:
	UFUNCTION()
	void Release(UParticleSystemComponent* PSystem);
};
