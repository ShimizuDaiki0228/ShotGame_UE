// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

/**
 * 
 */
class TPS_SHOT_API MontageUtility
{
public:
	UFUNCTION()
	template<typename TCallback>
	static void SetMontageEndDelegateWithLambda(
		UAnimInstance* animInstance,
		UAnimMontage* montage,
		TCallback callback
	)
	{
		SetMontageEndDelegateWithLambda(animInstance, montage, []() {return true;}, callback);
	}
	
	UFUNCTION()
	template<typename TWhere ,typename TCallback>
	static void SetMontageEndDelegateWithLambda(
		UAnimInstance* animInstance,
		UAnimMontage* montage,
		TWhere where,
		TCallback callback
	)
	{
		if (!animInstance || !montage)
		{
			return;
		}

		FOnMontageEnded montageEndedDelegate;
		montageEndedDelegate.BindLambda([where, callback](UAnimMontage* montage, bool bInterrupted)
		{
			UE_LOG(LogTemp, Log, TEXT("Where Check"));
			if (where() || !where)
			{
				UE_LOG(LogTemp, Log, TEXT("Callback Check"));
				if (callback)
				{
					callback(montage, bInterrupted);
				}
			}
		});

		animInstance->Montage_SetEndDelegate(montageEndedDelegate, montage);
	}
};
