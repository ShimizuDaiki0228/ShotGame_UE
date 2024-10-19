// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Engine/World.h"

/**
 * 
 */
class TPS_SHOT_API TimeManagerUtility
{
public:
	static TimeManagerUtility& GetInstance();

	template<typename T>
	void Delay(UWorld* world, T* object, void(T::* Method)(), float delayTime, FTimerHandle& timerHandle)
	{
		if (world)
		{
			world->GetTimerManager().SetTimer(timerHandle, object, Method, delayTime, false);
		}
	}

	template<typename T, typename... TArgs>
	void Delay(UWorld* world, T* object, void(T::* Method)(TArgs...), float delayTime, FTimerHandle& timerHandle, TArgs... args)
	{
		if (world)
		{
			FTimerDelegate timerDelegate;
			timerDelegate.BindLambda([=]()
				{
					(object->*Method)(args...);
				});

			
			world->GetTimerManager().SetTimer(timerHandle, timerDelegate, delayTime, false);
		}
	}

	void Delay(UWorld* world, TFunction<void()> callback, float delayTime, FTimerHandle& timerHandle);

	void Cancel(UWorld* world, FTimerHandle& timerHandle)
	{
		if (world)
		{
			world->GetTimerManager().ClearTimer(timerHandle);
		}
	}

private:
	TimeManagerUtility() {}
	
	TimeManagerUtility(const TimeManagerUtility&) = delete;
	TimeManagerUtility& operator=(const TimeManagerUtility&) = delete;
};
