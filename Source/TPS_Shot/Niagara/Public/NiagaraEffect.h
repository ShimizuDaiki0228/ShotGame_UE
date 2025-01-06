// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "TPS_Shot/DesignPattern/ObjectPattern/Public/PooledObject.h"
#include "NiagaraEffect.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API ANiagaraEffect : public ANiagaraActor, public IIPooledObject
{
	GENERATED_BODY()

public:
	ANiagaraEffect();

	UPROPERTY()
	AObjectPoolActor* Pool;

	FORCEINLINE void Release()
	{
		Pool->ReturnToPool(this);
	}
};
