// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObserver.h"


class TPS_SHOT_API IISubject
{
public:
	virtual ~IISubject() {}
	virtual void Attach(IIObserver* observer) = 0;
	virtual void Detach(IIObserver* observer) = 0;
	virtual void Notify() = 0;
};