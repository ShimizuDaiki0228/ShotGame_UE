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

	static FORCEINLINE float EaseInSine(float t)
	{
		return 1.0f - FMath::Cos(t * PI / 2.0f);
	}

	static FORCEINLINE float EaseOutCubic(float t)
	{
		return 1.0f - FMath::Pow(1.0f - t, 3);
	}

	static FORCEINLINE float EaseInOutQuart(float t)
	{
		return t < 0.5f ? 8.0f * FMath::Pow(t, 4) : 1.0f - FMath::Pow(-2.0f * t + 2.0f, 4) / 2.0f;
	}

	static FORCEINLINE FVector CalculateBezierPoint(float t, FVector p0, FVector p1, FVector p2, FVector p3, FVector p4)
	{
		float u = 1 - t;
		float tt = FMath::Pow(t, 2);
		float uu = FMath::Pow(u, 2);
		float ttt = FMath::Pow(t, 3);
		float uuu = FMath::Pow(u, 3);
		float tttt = FMath::Pow(t, 4);
		float uuuu = FMath::Pow(u, 4);

		FVector point = uuuu * p0;
		point += 4 * uuu * t * p1;
		point += 6 * uu * tt * p2;
		point += 4 * u * ttt * p3;
		point += tttt * p4;

		return point;
	}

private:
	// インスタンス化防止のためにコンストラクタを非公開
	EasingAnimationUtility() {}
};
