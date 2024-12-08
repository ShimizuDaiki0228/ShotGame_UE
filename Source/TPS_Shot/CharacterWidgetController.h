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
	void Initialized(AActor* owner, APlayerController* playerController);
	bool IsCharacterProjected(FVector2D& screenPosition);

public:
	// 現状高さは固定で考える
	void SetWidgetSetting(UUserWidget* userWidget,
	                             float widthClampSizeMin,
	                             float widthClampSizeMax,
	                             float heightSize);

	void SetTextSetting(UUserWidget* text,
						const float& widthClampSizeMin,
						const float& widthClampSizeMax,
						const bool bIsRandomPos = false);

private:
	UPROPERTY()
	AActor* _cachedOwner;

	UPROPERTY()
	APlayerController* _cachedPlayerController;

private:
	const float TEXT_POSITION_OFFSET_X = 150.0f;
	const float TEXT_POSITION_OFFSET_Y = 50.0f;
};
