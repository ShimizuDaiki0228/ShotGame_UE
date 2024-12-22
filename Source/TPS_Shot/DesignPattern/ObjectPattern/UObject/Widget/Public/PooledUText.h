// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Public/UPooledObjectBase.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "TPS_Shot/Widget/Public/IFollowActorWidget.h"
#include "PooledUText.generated.h"

class AUTextPoolActor;
class UCharacterWidgetController;

/**
 * UserWidgetを最初に1つ作ってしまって、そこにTextを量産していく
 * UserWidgetを作り直す方法だとうまくいかないため
 */
UCLASS()
class TPS_SHOT_API UPooledUText : public UUserWidget, public UPooledObjectBase<AUTextPoolActor, UPooledUText>,
								  public IIFollowActorWidget
{
	GENERATED_BODY()

public:
	void Initialized(AUTextPoolActor* poolActor,
		const UCanvasPanel* canvasPanel,
		TWeakObjectPtr<APlayerController> playerController);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SettingTextContents(const FString& text, UCharacterWidgetController* widgetController);

	void Release();

private:
	UPROPERTY()
	UTextBlock* _text;
	
	UPROPERTY()
	UCanvasPanelSlot* _canvasSlot;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> _cachedPlayerController;

	FSlateFontInfo _fontInfo;

	float _elapsedTime;

	FVector2D _screenPosition;
	
private:
	const float FADE_DURATION = 2.0f;
	// テキストが表示されてからどれくらい上昇するか
	const float MAX_FLOATING_AMOUNT = 70.0f;

private:
	virtual void SetSize(const FVector2D& size) override;
};
