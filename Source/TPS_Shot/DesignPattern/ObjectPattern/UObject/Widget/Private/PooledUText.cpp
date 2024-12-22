// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/PooledUText.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "../Public/UTextPoolActor.h"
#include "Components/CanvasPanelSlot.h"
#include "../../../Public/UPooledObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_Shot/Public/CharacterWidgetController.h"
#include "TPS_Shot/Utility/Public/EasingAnimationUtility.h"


void UPooledUText::Initialized(AUTextPoolActor* poolActor,
                               const UCanvasPanel* canvasPanel,
                               TWeakObjectPtr<APlayerController> playerController)
{
	_cachedPlayerController = playerController;
	if (!::IsValid(canvasPanel))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Canvas Panel is Null!!!!"), true, true, FColor::Red);
		return;
	}
	
	_text = Cast<UTextBlock>(GetWidgetFromName("pooledText"));
	if (::IsValid(_text))
	{
		_fontInfo = _text->GetFont();
		_canvasSlot = Cast<UCanvasPanelSlot>(_text->Slot);
		if (::IsValid(_canvasSlot))
		{
			// 正しいのかはわからないが、(0, 0)にすると、目的オブジェクトの画面中心座標に表示されるようになる
			_canvasSlot->SetSize(FVector2D(0.f, 0.f));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Failed to add TextBlock to CanvasPanel"), true, true, FColor::Red);
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Failed to create TextBlock"), true, true, FColor::Red);
	}

}

void UPooledUText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_elapsedTime += InDeltaTime;

	if (_text)
	{
		float fadeDurationRation = (static_cast<float>(_elapsedTime) / static_cast<float>(FADE_DURATION));
		float opacity = 1.0f - EasingAnimationUtility::EaseInOutQuart(fadeDurationRation);
		float floatingAmount = MAX_FLOATING_AMOUNT * EasingAnimationUtility::EaseInOutQuart(fadeDurationRation);

		_text->SetRenderOpacity(opacity);
		SetPositionInViewport(_screenPosition - FVector2D(0, floatingAmount), true);

		if (opacity <= 0.1f)
		{
			Release();
		}
	}

}

void UPooledUText::SettingTextContents(const FString& text, UCharacterWidgetController* widgetController)
{
	if (!_text)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("text is null"), true, true, FColor::Red);
		return;
	}
	
	if (::IsValid(_canvasSlot))
	{
		_text->SetText(FText::FromString(text));
		_screenPosition = widgetController->SetWidgetSetting(this, 30, 100, 30, 100, true);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Widget initialized successfully"));
}

void UPooledUText::Release()
{
	TWeakObjectPtr<UPooledUText> weakThis = TWeakObjectPtr<UPooledUText>(this);
	Pool->ReturnToPool(weakThis);
}

void UPooledUText::SetSize(const FVector2D& size)
{
	// 一応固定値にはしている。今後どうするか
	_fontInfo.Size = 30;
}
