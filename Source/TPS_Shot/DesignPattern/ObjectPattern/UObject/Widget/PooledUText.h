// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../UPooledObjectBase.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "PooledUText.generated.h"

class AUTextPoolActor;
class UCharacterWidgetController;

/**
 * UserWidgetを最初に1つ作ってしまって、そこにTextを量産していく
 * UserWidgetを作り直す方法だとうまくいかないため
 */
UCLASS()
class TPS_SHOT_API UPooledUText : public UUserWidget, public UPooledObjectBase<AUTextPoolActor, UPooledUText>
{
	GENERATED_BODY()

public:
	void Initialized(AUTextPoolActor* poolActor,
		const UCanvasPanel* canvasPanel,
		TWeakObjectPtr<APlayerController> playerController);

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
};
