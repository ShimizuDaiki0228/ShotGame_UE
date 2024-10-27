// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include <memory>
#include "../EnemyState/IExplosionEnemyState.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "../Niagara/ExplosionEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "ExplosionEnemyActor.generated.h"

//class ExplosionEnemyIdleState;
class IExplosionEnemyState;

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AExplosionEnemyActor : public AEnemyActor
{
	GENERATED_BODY()

public:
	AExplosionEnemyActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SelfDestroy() override;

private:
	// ‘¼‚Å‹¤—L‚µ‚È‚¢‚½‚ßunique_ptr‚Å
	std::unique_ptr<IExplosionEnemyState> _currentState;

public:
	virtual void Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager) override;
	
	void ChangeState(IExplosionEnemyState* newState);
};
