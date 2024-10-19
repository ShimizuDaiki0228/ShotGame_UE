// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManagerUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimeManagerUtility.h"

SoundManagerUtility& SoundManagerUtility::GetInstance()
{
	static SoundManagerUtility Instance;
	return Instance;
}

void SoundManagerUtility::Play(USoundBase* sound, UObject* object)
{
	if (!sound || !object)
	{
		return;
	}

	UAudioComponent*  audioComponent = UGameplayStatics::SpawnSound2D(object, sound);

	if (audioComponent)
	{
		/*float soundDuration = sound->GetDuration();

		TimeManagerUtility::GetInstance().Delay(object->GetWorld(), [this, audioComponent, object]
			{
				
				Stop(audioComponent);
			}, soundDuration);*/
	}
}

void SoundManagerUtility::Stop(UAudioComponent* audioComponent)
{

}

