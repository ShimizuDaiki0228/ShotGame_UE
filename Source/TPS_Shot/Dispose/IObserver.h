// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"

class TPS_SHOT_API IIObserver
{
public:
	virtual ~IIObserver() {}
	virtual void OnNext() = 0;
};