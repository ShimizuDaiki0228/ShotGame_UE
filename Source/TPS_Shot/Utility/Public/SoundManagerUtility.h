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
	// map�ł��悳���������A�����I�u�W�F�N�g���畡���̉����o���肻�̋t�̏ꍇ���l�����\�����ʉ��̒�~������邽��
	TArray<FTimerHandle> _timerHandles;
};
