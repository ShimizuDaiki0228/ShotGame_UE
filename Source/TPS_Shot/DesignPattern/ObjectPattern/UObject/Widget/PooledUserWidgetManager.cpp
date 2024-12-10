// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledUserWidgetManager.h"
#include "PooledUText.h"
#include "Blueprint/WidgetTree.h"

void UPooledUserWidgetManager::NativeConstruct()
{
	Super::NativeConstruct();
	
	_canvasPanel = Cast<UCanvasPanel>(GetWidgetFromName("CanvasPanel"));
	if (!IsValid(_canvasPanel))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("canvasPanel isn't valid"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledUText> UPooledUserWidgetManager::PooledTextCreate(
	AUTextPoolActor* pooledObjectBase,
	TSubclassOf<UPooledUText> pooledObjectClass,
	TWeakObjectPtr<APlayerController> playerController)
{
	UPooledUText* pooledText = CreateWidget<UPooledUText>(GetWorld(), pooledObjectClass);
	if (::IsValid(pooledText))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pooledText is valid"), true, true, FColor::Blue);
		pooledText->AddToViewport();
		pooledText->Initialized(pooledObjectBase, _canvasPanel, playerController);
		pooledText->Pool = pooledObjectBase;
		return pooledText;
	}

	UKismetSystemLibrary::PrintString(this, TEXT("pooledText isn't valid"), true, true, FColor::Blue);
	return pooledText;
}


