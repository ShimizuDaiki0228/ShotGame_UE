// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorScreenSizeCalculator.generated.h"

UCLASS()
class TPS_SHOT_API UActorScreenSizeCalculator : public UObject
{
	GENERATED_BODY()

public:
	static FVector2D CalculateScreenSize(const AActor* actor, const APlayerController* playerController);

private:
	static bool GetScreenBounds(const AActor* actor, const APlayerController* playerController, FVector2D& outMin, FVector2D& outMax);

private:
	static constexpr float BASE_SIZE = 200.0f; // スクリーン上の基準となる幅（ピクセル）
	static constexpr float SCALE_FACTOR = 1000.0f; // カメラ距離補正のスケール
};
