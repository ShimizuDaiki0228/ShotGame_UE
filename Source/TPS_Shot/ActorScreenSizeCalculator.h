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
	static float CalculateScreenSize(AActor* actor, APlayerController* playerController, float clampMin, float clampMax);

private:
	static bool GetScreenBounds(AActor* actor, APlayerController* playerController, FVector2D& outMin, FVector2D& outMax);

private:
	static constexpr float BASE_SIZE = 200.0f; // スクリーン上の基準となる幅（ピクセル）
	static constexpr float SCALE_FACTOR = 1000.0f; // カメラ距離補正のスケール
};
