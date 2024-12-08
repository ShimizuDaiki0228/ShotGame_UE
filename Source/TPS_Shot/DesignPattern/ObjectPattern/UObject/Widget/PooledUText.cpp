// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledUText.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "UTextPoolActor.h"
#include "Components/CanvasPanelSlot.h"
#include "../../UPooledObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_Shot/CharacterWidgetController.h"


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
	if (_text)
	{
		_canvasSlot = Cast<UCanvasPanelSlot>(_text->Slot);
		if (::IsValid(_canvasSlot))
		{
			// 正しいのかはわからないが、(0, 0)にすると、目的オブジェクトの画面中心座標に表示されるようになる
			_canvasSlot->SetSize(FVector2D(0.f, 0.f));
			UKismetSystemLibrary::PrintString(this, TEXT("TextBlock added to CanvasPanel"), true, true, FColor::Green);
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
		FSlateFontInfo FontInfo = _text->GetFont();
		FontInfo.Size = 80;
		_text->SetFont(FontInfo);
		widgetController->SetTextSetting(this, 30, 100, true);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Widget initialized successfully"));
}

void UPooledUText::Release()
{
	TWeakObjectPtr<UPooledUText> weakThis = TWeakObjectPtr<UPooledUText>(this);
	Pool->ReturnToPool(weakThis);
}
