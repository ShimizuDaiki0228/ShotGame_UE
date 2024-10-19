// Fill out your copyright notice in the Description page of Project Settings.


#include "EasingAnimationUtility.h"

EasingAnimationUtility& EasingAnimationUtility::GetInstance()
{
	static EasingAnimationUtility Instance;
	return Instance;
}

