// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/UTextPoolActor.h"

#include "../Public/PooledUserWidgetManager.h"
#include "../Public/PooledUText.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

using namespace UE::Tasks;

void AUTextPoolActor::BeginPlay()
{
	Super::BeginPlay();

	_cachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);;
	
	_widgetManager = CreateWidget<UPooledUserWidgetManager>(GetWorld(), _pooledWidgetManagerClass);
	
	if (_widgetManager.IsValid())
	{
		_widgetManager->AddToViewport();
		_widgetManager->SetVisibility(ESlateVisibility::Visible);
		
		FTask widgetManagerConstructTask = Launch
		(TEXT("widgetManagerConstruct Busy Wait"), [this]()
			{
				while (!_widgetManager->IsConstructed())
				{
					FPlatformProcess::Sleep(0.1f);
				}
		
				_pooledUTextClass = TSubclassOf<UPooledUText>(*_pooledWidgetClass);

				// コンストラクト完了後の処理
				for (int i = 0; i < _initPoolSize; i++)
				{
					auto pooledText = CreateNewPooledObject();
					pooledText->Release();
				}
			}
		);
		widgetManagerConstructTask.BusyWait();
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("widget Manager isn't valid"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledUText> AUTextPoolActor::GetPooledObject(
	const FString& text,
	UCharacterWidgetController* widgetController)
{
	TWeakObjectPtr<UPooledUText> pooledText = GetPooledWidgetBase<UPooledUText>();
	
	if (pooledText.IsValid())
	{
		pooledText = CreateNewPooledObject();
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pooledText doesn't create"), true, true, FColor::Cyan);

	}

	pooledText->SettingTextContents(text, widgetController);
	pooledText->SetIsEnabled(true);
	
	return pooledText;
}

void AUTextPoolActor::ReturnToPool(TWeakObjectPtr<UPooledUText> pooledText)
{
	if (pooledText.IsValid())
	{
		pooledText->SetIsEnabled(false);
		pooledText->SetVisibility(ESlateVisibility::Collapsed);
		ReturnToPoolWidgetBase(pooledText);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pool object isn't vaid"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledUText> AUTextPoolActor::CreateNewPooledObject()
{
	TWeakObjectPtr<UPooledUText> pooledText = _widgetManager->TextInitialized(this, _pooledUTextClass, _cachedPlayerController);
	
	if (pooledText.IsValid())
	{
		_pooledWidgetStack.Add(pooledText);
		return pooledText;
	}
	
	UKismetSystemLibrary::PrintString(this, TEXT("pooledText isn't valid"), true, true, FColor::Red);
	return nullptr;
}
