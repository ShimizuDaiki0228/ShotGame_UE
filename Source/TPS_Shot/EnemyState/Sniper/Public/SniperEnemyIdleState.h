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

	// ��������Ă���w�肳�ꂽ�ʒu�ɍs���܂łɂ����鎞��
	const float APPEAR_DURATION = 2.0f;

	

	// �ŏ��̎w��ʒu�Ɉړ�����܂ł̏���
	bool SetFirstPosition(ASniperEnemyActor* enemy);

	const FVector FIRST_POSITION_OFFSET_Z = FVector(0, 0, 300);

};
