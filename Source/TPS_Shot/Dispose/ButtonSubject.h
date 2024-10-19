// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h" 
#include "ButtonSubject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClickDelegate);

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UButtonSubject : public UButton
{
	GENERATED_BODY()
	
public:
	UButtonSubject()
	{
		OnClicked.AddDynamic(this, &UButtonSubject::HandleOnClick);
	}

	template<typename Callable>
	void Subscribe(Callable OnClickCallback)
	{
		_onClickDelegate.AddLambda(OnClickCallback);
	}

	void Dispose()
	{
		_onClickDelegate.Clear();
	}

private:

	FOnClickDelegate _onClickDelegate;

	UFUNCTION()
	void HandleOnClick()
	{
		_onClickDelegate.Broadcast();
	}
};
