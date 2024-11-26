// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBarUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ProgressBar.h"

void UEnemyHpBarUserWidget::UpdateHpBar(float newHpPercentage)
{
	UKismetSystemLibrary::PrintString(this, TEXT("Change"), true, true, FColor::Red);

	if (_hpBar != nullptr)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("currentHP : %f"), newHpPercentage), true, true, FColor::Green);
		_hpBar->SetPercent(newHpPercentage);
	}
}

void UEnemyHpBarUserWidget::SetSize(UCanvasPanelSlot* canvasSlot, float width, float height)
{
	if (canvasSlot != nullptr)
	{
		FVector2D healthBarSize = FVector2D(width, canvasSlot->GetSize().Y);
		canvasSlot->SetSize(healthBarSize);
	}
}
