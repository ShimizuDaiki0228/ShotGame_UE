// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHUD.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHUD : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPS_SHOT_API IIHUD
{
	GENERATED_BODY()

public:
	virtual void SetHP(float percent) = 0;
};
