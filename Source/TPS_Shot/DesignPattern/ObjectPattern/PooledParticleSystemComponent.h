// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleSystemPoolActor.h"
#include "PooledParticleSystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UPooledParticleSystemComponent : public UParticleSystemComponent
{
	GENERATED_BODY()
	
public:
	UPooledParticleSystemComponent();

private:
	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* PSystem);

public:
	UPROPERTY()
	AParticleSystemPoolActor* Pool;

	// �������瑱��
	void Release();
};
