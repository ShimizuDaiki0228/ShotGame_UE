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

public:
	
	
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _hpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _damageBar;


private:
	FTimerHandle _damageBarAnimationTimerHandle;

	UPROPERTY()
	TArray<UCanvasPanelSlot*> _canvasSlots;

	// 赤ゲージが残っているときにダメージを食らったときにゲージアニメーションをいったん終わらせるため
	float _cacheNewHpValue;

	bool bTimerHandleActive = false;

public:
	void ManualBeginPlay();
	
	UFUNCTION()
	void UpdateHpBar(float newHpPercentage);
	
	UProgressBar* GetHpBar() const {return _hpBar;}

	virtual void SetSize(const FVector2D& size) override;

public:
	static constexpr float HPBAR_CLAMP_SIZE_MIN = 150;
	static constexpr float HPBAR_CLAMP_SIZE_MAX = 300;
};
