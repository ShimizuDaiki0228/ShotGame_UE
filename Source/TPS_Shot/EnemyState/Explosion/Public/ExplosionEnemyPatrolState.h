// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../IExplosionEnemyState.h"
#include "../../Public/IAttackable.h"

/**
 * 
 */
class TPS_SHOT_API ExplosionEnemyPatrolState : public IExplosionEnemyState, public IIAttackable
{
public:
	virtual void EnterState(AExplosionEnemyActor* enemy) override;
	virtual void UpdateState(AExplosionEnemyActor* enemy) override;
	virtual void ExitState(AExplosionEnemyActor* enemy) override;

private:
	float _moveDirection;

	void TransitionToTracking(AExplosionEnemyActor* enemy);
	const float STOP_THREASHOLD = 10.0f;

	bool bIsTransitioning;

	void SetVelocity(float decelerationRate, AExplosionEnemyActor* enemy);

protected:
	virtual bool CheckAndAttack(AExplosionEnemyActor* enemy) override;
};
