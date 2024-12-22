// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/Public/SniperEnemyActor.h"

class ASniperEnemyActor;

/**
 * 
 */
class TPS_SHOT_API ISniperEnemyState
{
public:
	virtual ~ISniperEnemyState() {}

	// �������z�֐�
	virtual void EnterState(ASniperEnemyActor* enemy) = 0;
	virtual void UpdateState(ASniperEnemyActor* enemy) = 0;
	virtual void ExitState(ASniperEnemyActor* enemy) = 0;
};
