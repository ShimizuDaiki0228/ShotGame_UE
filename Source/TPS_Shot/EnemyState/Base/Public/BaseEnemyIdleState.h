// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Public/IEnemyState.h"


/**
 * 
 */
class TPS_SHOT_API BaseEnemyIdleState : public IEnemyState
{
public:
	//virtual ~BaseEnemyIdleState(){}

	virtual void EnterState(AEnemyActor* enemy) override;
	virtual void UpdateState(AEnemyActor* enemy) override;
	virtual void ExitState(AEnemyActor* enemy) override;
};
