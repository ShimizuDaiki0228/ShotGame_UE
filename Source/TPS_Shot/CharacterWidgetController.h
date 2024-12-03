// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UCharacterWidgetController : public UObject
{
	GENERATED_BODY()

public:
	// 現状高さは固定で考える
	static void SetWidgetSetting(AActor* owner,
	                             UUserWidget* userWidget,
	                             APlayerController* playerController,
	                             float widthClampSizeMin,
	                             float widthClampSizeMax,
	                             float heightSize);
};
