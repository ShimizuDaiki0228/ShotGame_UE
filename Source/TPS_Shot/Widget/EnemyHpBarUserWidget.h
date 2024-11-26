// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IFollowActorWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UEnemyHpBarUserWidget : public UUserWidget, public IIFollowActorWidget
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

	virtual void SetSize(UCanvasPanelSlot* canvasSlot, float width, float height) override;

	static constexpr float HPBAR_CLAMP_SIZE_MIN = 150;
	static constexpr float HPBAR_CLAMP_SIZE_MAX = 300;
};
