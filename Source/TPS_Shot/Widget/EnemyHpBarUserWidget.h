// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UEnemyHpBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _hpBar;

public:
	UFUNCTION()
	void UpdateHpBar(float newHpPercentage);

	void Test(float newHpPercentage);

	UProgressBar* GetHpBar() const {return _hpBar;}
};
