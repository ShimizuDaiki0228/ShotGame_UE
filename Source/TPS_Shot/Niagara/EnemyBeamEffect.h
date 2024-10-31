// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraEffect.h"
#include "EnemyBeamEffect.generated.h"

/**
 * 
 */

// いったん使わないため放置、強化された敵が出たとき
UCLASS()
class TPS_SHOT_API AEnemyBeamEffect : public ANiagaraEffect
{
	GENERATED_BODY()
	
public:
	/*virtual void Tick(float DeltaTime) override;

	void SetParams(const FVector& targetLocation, const FVector& firingLocation);*/

private:
	//FVector _targetLocation;

	//FVector _firingLocation;

	//// TickのほうがSetParamsより早い可能性があるため
	//bool bCanShot;

	//UNiagaraComponent* _effectSystem;
};
