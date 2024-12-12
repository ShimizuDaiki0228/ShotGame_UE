// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"

template<typename... TArgs>
class TPS_SHOT_API IIObserver
{
public:
	virtual ~IIObserver() {}
	virtual void SetSharedPtr(TWeakPtr<IIObserver<TArgs...>> sharedObserver) = 0;
	virtual void OnNext(TArgs... args) = 0;
	virtual void RemoveObserver() = 0;
};