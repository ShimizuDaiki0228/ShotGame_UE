// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetUtility.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UWidgetUtility : public UObject
{
	GENERATED_BODY()

public:
	static UWidgetUtility* GetInstance();

	static void SetWidgetScale(
		FVector2D& ownerScreenSize,
		const AActor* owner,
		UUserWidget* widget,
		const APlayerController* playerController,
		const float widthClampSizeMin = 0,
		const float widthClampSizeMax = 0,
		const float heightClampSizeMin = 0,
		const float heightClampSizeMax = 0);

	// 画面に表示する際の大きさ設定
	static void SetWidgetPosition(
		UUserWidget* widget,
		const FVector2D& screenPosition);

private:
	static UWidgetUtility* _instance;
};
