// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjectPooledSystemBase.h"
#include "Kismet/KismetSystemLibrary.h"

class AUObjectPooledSystemBase;

/**
 * 
 */
template<typename TObjectPoolSystem, typename TObjectClass>
class TPS_SHOT_API UPooledObjectBase
{
public:
	UPooledObjectBase() : Pool(nullptr) {}
	virtual ~UPooledObjectBase() = default;

	UPROPERTY()
	TObjectPoolSystem* Pool;

protected:
	// void ReleaseBase(TWeakObjectPtr<TObjectClass> pooledObject) const
	// {
	// 	if (AUObjectPooledSystemBase* PooledSystem = static_cast<AUObjectPooledSystemBase*>(Pool))
	// 	{
	// 		PooledSystem->ReturnToPoolBase<TObjectClass>(pooledObject);
	// 	}
	// 	else
	// 	{
	// 		UKismetSystemLibrary::PrintString(pooledObject.Get(), TEXT("can't cast to UObjectPooledSystemBase"), true, true, FColor::Red);
	// 	}
	// }
	//
	// void ReleaseWidgetBase(TWeakObjectPtr<TObjectClass> pooledObject) const
	// {
	// 	if (AUObjectPooledSystemBase* PooledSystem = static_cast<AUObjectPooledSystemBase*>(Pool))
	// 	{
	// 		PooledSystem->ReturnToPoolWidgetBase(pooledObject);
	// 	}
	// 	else
	// 	{
	// 		UKismetSystemLibrary::PrintString(pooledObject.Get(), TEXT("can't cast to UObjectPooledSystemBase"), true, true, FColor::Red);
	// 	}
	// }
};
