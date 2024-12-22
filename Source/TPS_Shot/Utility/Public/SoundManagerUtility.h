// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/Delegate.h"
#include "Components/AudioComponent.h"


/**
 * 
 */
class TPS_SHOT_API SoundManagerUtility
{
public:
	static SoundManagerUtility& GetInstance();

	void Play(USoundBase* sound, UObject* object);

	void Stop(UAudioComponent* object);

private:
	SoundManagerUtility() {}

	SoundManagerUtility(const SoundManagerUtility&) = delete;
	SoundManagerUtility& operator=(const SoundManagerUtility&) = delete;

private:
	// mapでもよさそうだが、同じオブジェクトから複数の音が出たりその逆の場合も考えられ予期せぬ音の停止を避けるため
	TArray<FTimerHandle> _timerHandles;
};
