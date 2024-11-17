// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolActor.h"
#include "PooledObjectActor.generated.h"


UCLASS()
class TPS_SHOT_API APooledObjectActor : public AActor
{
	GENERATED_BODY()

public:
	AObjectPoolActor* Pool;

	FORCEINLINE void Release()
	{
		Pool->ReturnToPool(this);
	}
};
