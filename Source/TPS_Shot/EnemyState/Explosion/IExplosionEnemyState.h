// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/Public/ExplosionEnemyActor.h"

class AExplosionEnemyActor;

/**
 * 
 */
class TPS_SHOT_API IExplosionEnemyState
{
public:
	virtual ~IExplosionEnemyState() {}

	virtual void EnterState(AExplosionEnemyActor* enemy) = 0;
	virtual void UpdateState(AExplosionEnemyActor* enemy) = 0;
	virtual void ExitState(AExplosionEnemyActor* enemy) = 0;
};
