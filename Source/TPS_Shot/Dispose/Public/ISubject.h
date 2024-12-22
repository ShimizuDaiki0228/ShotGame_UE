// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObserver.h"

template<typename... TArgs>
class TPS_SHOT_API IISubject
{
public:
	virtual ~IISubject() {}
	virtual void ManualAttach(TWeakPtr<IIObserver<TArgs...>> observer) = 0;
	virtual void Detach(TWeakPtr<IIObserver<TArgs...>> observer) = 0;
	virtual void Notify(TArgs...) = 0;
	virtual void ShutDown() = 0;
};