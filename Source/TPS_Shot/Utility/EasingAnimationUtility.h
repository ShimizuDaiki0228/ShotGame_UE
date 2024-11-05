// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TPS_SHOT_API EasingAnimationUtility
{
public:
	static FORCEINLINE float EaseInCirc(float t)
	{
		return 1.0f - FMath::Sqrt(1.0f - FMath::Square(t));
	}

	static FORCEINLINE float EaseInQuart(float t)
	{
		return FMath::Pow(t, 4);
	}

	static FORCEINLINE float EaseOutCubic(float t)
	{
		return 1.0f - FMath::Pow(1.0f - t, 3);
	}

	static FORCEINLINE float EaseInOutQuart(float t)
	{
		return t < 0.5f ? 8.0f * FMath::Pow(t, 4) : 1.0f - FMath::Pow(-2.0f * t + 2.0f, 4) / 2.0f;
	}

	static FORCEINLINE FVector CalculateBezierPoint(float t, FVector p0, FVector p1, FVector p2, FVector p3)
	{
		float u = 1 - t;
		float tt = FMath::Pow(t, 2);
		float uu = FMath::Pow(u, 2);
		float ttt = FMath::Pow(t, 3);
		float uuu = FMath::Pow(u, 3);

		FVector point = uuu * p0;
		point += 3 * uu * t * p1;
		point += 3 * u * tt * p2;
		point += ttt * p3;

		return point;
	}

private:
	// インスタンス化防止のためにコンストラクタを非公開
	EasingAnimationUtility() {}
};
