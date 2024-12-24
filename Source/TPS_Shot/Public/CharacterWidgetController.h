// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterWidgetController.generated.h"

class AMyPlayerControllerBase;
/**
 * 
 */
UCLASS()
class TPS_SHOT_API UCharacterWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void Initialized(AActor* owner);
	bool IsCharacterProjected(FVector2D& screenPosition) const;

public:
	// 現状高さは固定で考える
	FVector2D SetWidgetSetting(UUserWidget* userWidget,
	                             float widthClampSizeMin = 0,
	                             float widthClampSizeMax = 0,
	                             float heightClampSizeMin = 0,
	                             float heightClampSizeMax = 0,
								 const bool bIsRandomPos = false);

private:
	UPROPERTY()
	AActor* _cachedOwner;

	UPROPERTY()
	APlayerController* _cachedPlayerController;
};
