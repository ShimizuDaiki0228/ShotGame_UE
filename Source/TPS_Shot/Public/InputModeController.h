// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputModeController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UInputModeController : public UObject
{
	GENERATED_BODY()
	
public:
	UInputModeController();

	void Initialized(APlayerController* playerController);
};
