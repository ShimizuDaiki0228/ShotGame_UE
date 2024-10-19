// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../IExplosionEnemyState.h"

/**
 * 
 */
class TPS_SHOT_API ExplosionEnemyTrackingState : public IExplosionEnemyState
{
public:
	virtual void EnterState(AExplosionEnemyActor* enemy) override;
	virtual void UpdateState(AExplosionEnemyActor* enemy) override;
	virtual void ExitState(AExplosionEnemyActor* enemy) override;

private:
	const float EXPLODE_TIME = 3.0f;
	const float EXPLODE_INTERVAL_TIME = 0.1f;
	float _targetDistance;
	float CalculateTargetDistance(AExplosionEnemyActor* enemy);

	bool _updateEnable;
	void ChangeUpdateBrakeEnable(AExplosionEnemyActor* enemy);
	const float UPDATE_ENABLE_INTERVAL = 0.15f;

	/*void MaterialSetting(AExplosionEnemyActor* enemy);
	void StartColorChange(UMaterialInstanceDynamic* materialInstance,
						  AExplosionEnemyActor* enemy,
						  FLinearColor startColor,
						  FLinearColor endColor);
	void UpdateColor(UMaterialInstanceDynamic* materialInstance,
					 AExplosionEnemyActor* enemy,
					 FLinearColor startColor,
					 FLinearColor endColor);
	FLinearColor GetMaterialColor(UMaterialInterface* material, FName parameterName);*/
	UMaterialInstanceDynamic* _materialInstance;
	float _timeElapsed;
	const float COLOR_CHANGE_DURATION = 3.0f;

	FTimerHandle _brakeTimerHandle;

	FTimerHandle _explodeTimerHandle;

	TArray<FTimerHandle> _timerHandles;
};
