// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"

void UWidgetManager::ChangeViewPort(UUserWidget* newWidget, UUserWidget* deleteWidget)
{
	newWidget->AddToViewport();

	if (deleteWidget != nullptr)
	{
		deleteWidget->RemoveFromParent();
	}
}