// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPooledObject.generated.h"

class AObjectPoolActor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPooledObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPS_SHOT_API IIPooledObject
{
	GENERATED_BODY()

public:
	AObjectPoolActor* Pool;

	virtual void Release() = 0;
};
