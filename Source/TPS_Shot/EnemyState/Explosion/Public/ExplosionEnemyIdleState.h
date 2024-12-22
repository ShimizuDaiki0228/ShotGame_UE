// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../IExplosionEnemyState.h"
#include "../../Public/IAttackable.h"

/**
 * 
 */
class TPS_SHOT_API ExplosionEnemyIdleState : public IExplosionEnemyState, public IIAttackable
{
public:
	//virtual ~ExplosionEnemyIdleState() {}
	virtual void EnterState(AExplosionEnemyActor* enemy) override;
	virtual void UpdateState(AExplosionEnemyActor* enemy) override;
	virtual void ExitState(AExplosionEnemyActor* enemy) override;

private:
	float _waitTime;
	const float NEXT_MOVEMENT_TIME = 3.0f;

	void CalculateElapsedTime(AExplosionEnemyActor* enemy);

protected:
	virtual bool CheckAndAttack(AExplosionEnemyActor* enemy) override;
};
