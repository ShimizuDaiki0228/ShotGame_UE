// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPS_Shot/HUD/Public/IHUD.h"
#include "UserWidget_Playing.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UUserWidget_Playing : public UUserWidget, public IIHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _scoreText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* _hpBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _mainText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _numberOfBullet;

public:
	void UpdateScore(int newScore);

	void Reset(int score);

	void Gameover();

	UFUNCTION()
	void UpdateHPBar(float hpPercentage);

	UFUNCTION()
	void ChangeBulletNumber(int newBulletNumber);

	virtual void SetHP(float percent) override;
private:
	void SetScoreText(int newScore);
};
