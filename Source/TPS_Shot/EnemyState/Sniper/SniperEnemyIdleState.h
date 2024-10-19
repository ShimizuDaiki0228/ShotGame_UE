// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ISniperEnemyState.h"

/**
 * 
 */

class TPS_SHOT_API SniperEnemyIdleState : public ISniperEnemyState
{
public:
	void EnterState(ASniperEnemyActor* enemy);
	void UpdateState(ASniperEnemyActor* enemy);
	void ExitState(ASniperEnemyActor* enemy);

private:
	float _appearElapsedTime;

	// 生成されてから指定された位置に行くまでにかける時間
	const float APPEAR_DURATION = 2.0f;

	

	// 最初の指定位置に移動するまでの処理
	bool SetFirstPosition(ASniperEnemyActor* enemy);

	const FVector FIRST_POSITION_OFFSET_Z = FVector(0, 0, 300);

};
