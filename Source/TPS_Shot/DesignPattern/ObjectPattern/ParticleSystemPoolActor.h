// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleSystemPoolActor.generated.h"

class UPooledParticleSystemComponent;

UCLASS()
class TPS_SHOT_API AParticleSystemPoolActor : public AActor
{
	GENERATED_BODY()
	
public:
	AParticleSystemPoolActor();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, Category = "Pool")
	int _initPoolSize = 3;

	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<UPooledParticleSystemComponent*> _pooledObjectStack;

	UPROPERTY(EditAnywhere, Category = "Pool")
	UParticleSystem* _particleSystem;

public:
	UPooledParticleSystemComponent* GetPooledObject(const AActor* owner);

	void ReturnToPool(UPooledParticleSystemComponent* particleComponent);

private:
	UPooledParticleSystemComponent* CreateNewPooledObject();
};