// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AEnemyActor;

/**
 * 
 */
class TPS_SHOT_API IEnemyState
{
public:
	virtual ~IEnemyState() {}

	// èÉêàâºëzä÷êî
	virtual void EnterState(AEnemyActor* enemy) = 0;
	virtual void UpdateState(AEnemyActor* enemy) = 0;
	virtual void ExitState(AEnemyActor* enemy) = 0;
};
