// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

class AUObjectPooledSystemBase;

/**
 * 
 */
template<typename T>
class TPS_SHOT_API UPooledObjectBase
{
public:
	UPooledObjectBase() : Pool(nullptr) {}
	virtual ~UPooledObjectBase() = default;

	UPROPERTY()
	T* Pool;

private:
	void ReleaseBase()
	{
		if (AUObjectPooledSystemBase* PooledSystem = static_cast<AUObjectPooledSystemBase*>(Pool))
		{
			PooledSystem->ReturnToPool(this);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("can't cast to UObjectPooledSystemBase"), true, true, FColor::Purple);
		}
	}
};
