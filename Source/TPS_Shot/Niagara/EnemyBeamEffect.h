// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraEffect.h"
#include "EnemyBeamEffect.generated.h"

/**
 * 
 */

// ��������g��Ȃ����ߕ��u�A�������ꂽ�G���o���Ƃ�
UCLASS()
class TPS_SHOT_API AEnemyBeamEffect : public ANiagaraEffect
{
	GENERATED_BODY()
	
public:
	/*virtual void Tick(float DeltaTime) override;

	void SetParams(const FVector& targetLocation, const FVector& firingLocation);*/

private:
	//FVector _targetLocation;

	//FVector _firingLocation;

	//// Tick�̂ق���SetParams��葁���\�������邽��
	//bool bCanShot;

	//UNiagaraComponent* _effectSystem;
};
