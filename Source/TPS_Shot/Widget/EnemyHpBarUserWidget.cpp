// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBarUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Utility/TimeManagerUtility.h"
#include "Components/ProgressBar.h"

void UEnemyHpBarUserWidget::ManualBeginPlay()
{
	_canvasSlots.Add(Cast<UCanvasPanelSlot>(_hpBar->Slot));
	_canvasSlots.Add(Cast<UCanvasPanelSlot>(_damageBar->Slot));
}

void UEnemyHpBarUserWidget::UpdateHpBar(float newHpPercentage)
{
	if (_hpBar != nullptr)
	{
		_hpBar->SetPercent(newHpPercentage);
	}
	
	if (_damageBar != nullptr)
	{
		if (bTimerHandleActive)
		{
			TimeManagerUtility::GetInstance().Cancel(GetWorld(), _damageBarAnimationTimerHandle);
			_damageBar->SetPercent(_cacheNewHpValue);
			bTimerHandleActive = false;
		}
		
		bTimerHandleActive = true;
		TimeManagerUtility::GetInstance().Repeat(GetWorld(), [this, newHpPercentage]()
		{
			float currentDamagePercent = _damageBar->GetPercent();
			
			if (currentDamagePercent > newHpPercentage)
			{
				float newPercent = FMath::FInterpTo(currentDamagePercent, newHpPercentage, GetWorld()->GetDeltaSeconds(), 2.0f);
				_damageBar->SetPercent(newPercent);
			}
			else
			{
				TimeManagerUtility::GetInstance().Cancel(GetWorld(), _damageBarAnimationTimerHandle);
				bTimerHandleActive = false;
			}
		}, 0.016f, _damageBarAnimationTimerHandle);
	}
	_cacheNewHpValue = newHpPercentage;
}

void UEnemyHpBarUserWidget::SetSize(const FVector2D& size)
{
	for (auto canvasSlot : _canvasSlots)
	{
		if (::IsValid(canvasSlot))
		{
			FVector2D healthBarSize = FVector2D(size.X, canvasSlot->GetSize().Y);
			canvasSlot->SetSize(healthBarSize);
		}
	}
}
