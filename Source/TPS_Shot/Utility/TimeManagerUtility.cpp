// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManagerUtility.h"

TimeManagerUtility& TimeManagerUtility::GetInstance()
{
    static TimeManagerUtility Instance;
    return Instance;
}

void TimeManagerUtility::Delay(UWorld* World, TFunction<void()> Callback, float DelayTime, FTimerHandle& timerHandle)
{
    if (World)
    {
        World->GetTimerManager().SetTimer(timerHandle, [Callback]()
            {
                Callback();
            }, DelayTime, false);
    }
}