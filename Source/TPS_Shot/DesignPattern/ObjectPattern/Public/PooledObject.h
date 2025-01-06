// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPooledObject.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolActor.h"
#include "PooledObject.generated.h"

UCLASS()
class TPS_SHOT_API APooledObject : public AActor, public IIPooledObject
{
	GENERATED_BODY()

public:	
	UPROPERTY()
	AObjectPoolActor* Pool;

	FORCEINLINE void Release()
	{
		Pool->ReturnToPool(this);
	}
};
