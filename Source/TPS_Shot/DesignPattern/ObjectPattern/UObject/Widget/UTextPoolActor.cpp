// Fill out your copyright notice in the Description page of Project Settings.


#include "UTextPoolActor.h"

#include "NavigationSystemTypes.h"
#include "PooledUText.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Sections/MovieSceneLevelVisibilitySection.h"
#include "Kismet/KismetSystemLibrary.h"

void AUTextPoolActor::BeginPlay()
{
	Super::BeginPlay();

	if (_pooledObject != nullptr)
	{
		for (int i = 0; i < _initPoolSize; i++)
		{
			CreateNewPooledObject();
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pool object is nullptr"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledUText> AUTextPoolActor::GetPooledObject(const AActor* owner)
{
	TWeakObjectPtr<UPooledUText> pooledText = GetPooledObjectBase<UPooledUText>(owner);

	if (!pooledText.IsValid())
	{
		pooledText = CreateNewPooledObject();
	}
	
	pooledText->SetVisibility(ESlateVisibility::Visible);
	
	return pooledText;
}

void AUTextPoolActor::ReturnToPool(TWeakObjectPtr<UPooledUText> pooledText)
{
	if (pooledText.IsValid())
	{
		pooledText->SetVisibility(ESlateVisibility::Collapsed);
		ReturnToPoolBase(pooledText);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pool object isn't vaid"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledUText> AUTextPoolActor::CreateNewPooledObject()
{
	TWeakObjectPtr<UPooledUText> pooledText = Cast<UPooledUText>(_pooledObject);

	if (pooledText.IsValid())
	{
		UCanvasPanelSlot* canvasPanel = Cast<UCanvasPanelSlot>(pooledText->GetTextBlock()->Slot);
		if (canvasPanel != nullptr)
		{
			canvasPanel->SetPosition(FVector2D::ZeroVector);
			pooledText->SetVisibility(ESlateVisibility::Collapsed);
			pooledText->Pool = this;
			_pooledObjectStack.Add(pooledText);
			return pooledText;
		}

		UKismetSystemLibrary::PrintString(this, TEXT("canvasSlot is nullptr"), true, true, FColor::Red);
		return nullptr;
	}

	UKismetSystemLibrary::PrintString(this, TEXT("pooledObject isn't valid"), true, true, FColor::Red);
	return nullptr;
}
