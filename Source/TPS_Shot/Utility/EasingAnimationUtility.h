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

private:
	// インスタンス化防止のためにコンストラクタを非公開
	EasingAnimationUtility() {}
};
