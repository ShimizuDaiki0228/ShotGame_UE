// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledUText.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "UTextPoolActor.h"
#include "Components/CanvasPanelSlot.h"
#include "../../UPooledObjectBase.h"


void UPooledUText::Initialized(AUTextPoolActor* poolActor, const UCanvasPanel* canvasPanel)
{
	if (!::IsValid(canvasPanel))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Canvas Panel is Null!!!!"), true, true, FColor::Red);
		return;
	}
	
	_text = Cast<UTextBlock>(GetWidgetFromName("pooledText"));
	if (_text)
	{
		_text->SetText(FText::FromString(TEXT("pooledText")));
		FSlateFontInfo FontInfo = _text->GetFont();
		FontInfo.Size = 80;
		_text->SetFont(FontInfo);
	
		_canvasSlot = Cast<UCanvasPanelSlot>(_text->Slot);
		if (_canvasSlot)
		{
			_canvasSlot->SetPosition(FVector2D(300.f, 300.f));
			_canvasSlot->SetSize(FVector2D(300.f, 50.f));
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

void UPooledUText::SetText() const
{
	if (!_text)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("text is null"), true, true, FColor::Red);
		return;
	}
	
	if (_canvasSlot)
	{
		_canvasSlot->SetSize(FVector2D(200.f, 50.f)); // サイズ
		_canvasSlot->SetPosition(FVector2D(50.f, 50.f)); // 位置
	}
	
	UE_LOG(LogTemp, Log, TEXT("Widget initialized successfully"));
}

void UPooledUText::Release()
{
	TWeakObjectPtr<UPooledUText> weakThis = TWeakObjectPtr<UPooledUText>(this);
	Pool->ReturnToPool(weakThis);
}
