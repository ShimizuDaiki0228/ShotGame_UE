// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolUObject.generated.h"

class UPooledParticleSystemComponent;

// エフェクト用で考えている
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_SHOT_API UObjectPoolUObject : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolUObject();

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

	UPROPERTY(EditAnywhere, Category = "Pool")
	AActor* _owner;

	UPROPERTY(EditAnywhere, Category = "Components")
	UActorComponent* TargetComponent;

public:
	UPooledParticleSystemComponent* GetPooledObject();

	void ReturnToPool(UPooledParticleSystemComponent* particleComponent);

private:
	UPooledParticleSystemComponent* CreateNewPooledObject();

	void ActiveChange(UPooledParticleSystemComponent* pooledObjectComponent, bool isPopObject);

	USceneComponent* _rootComponent;

	void Test();

	UPooledParticleSystemComponent* aaa;
	FTimerHandle handle;

};
