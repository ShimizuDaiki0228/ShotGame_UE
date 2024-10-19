// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ISniperEnemyState.h"

/**
 * 
 */
class TPS_SHOT_API SniperEnemyPatrolState : public ISniperEnemyState
{
public:
	void EnterState(ASniperEnemyActor* enemy);
	void UpdateState(ASniperEnemyActor* enemy);
	void ExitState(ASniperEnemyActor* enemy);

private:
	AActor* _nextPos;

	float _moveElapsedTime;

	const float MOVE_DURATION = 4.0f;
};
