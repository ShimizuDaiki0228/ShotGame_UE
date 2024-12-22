// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObjectPooledSystemBase.h"
#include "ParticleSystemPoolActor.generated.h"

class UPooledParticleSystemComponent;

UCLASS()
class TPS_SHOT_API AParticleSystemPoolActor : public AUObjectPooledSystemBase 
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	TWeakObjectPtr<UPooledParticleSystemComponent> GetPooledObject(const AActor* owner);

	void ReturnToPool(TWeakObjectPtr<UPooledParticleSystemComponent> particleComponent);

private:
	bool SetParticleSystem(const TWeakObjectPtr<UPooledParticleSystemComponent>& pooledObject) const;

	TWeakObjectPtr<UPooledParticleSystemComponent> CreateNewPooledObject();
};