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

protected:
	void ReleaseBase(TWeakObjectPtr<> pooledObject) const
	{
		if (AUObjectPooledSystemBase* PooledSystem = static_cast<AUObjectPooledSystemBase*>(Pool))
		{
			PooledSystem->ReturnToPoolBase(pooledObject);
		}
		else
		{
			UKismetSystemLibrary::PrintString(pooledObject.Get(), TEXT("can't cast to UObjectPooledSystemBase"), true, true, FColor::Purple);
		}
	}
};
