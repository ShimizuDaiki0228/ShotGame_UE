// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/NoExportTypes.h"
#include "WidgetUtility.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UWidgetUtility : public UObject
{
	GENERATED_BODY()

public:
	static void SetWidgetScale(
		FVector2D& ownerScreenSize,
		const AActor* owner,
		UUserWidget* widget,
		const APlayerController* playerController,
		const float widthClampSizeMin = 0,
		const float widthClampSizeMax = 0,
		const float heightClampSizeMin = 0,
		const float heightClampSizeMax = 0);

	// 画面に表示する際の大きさ設定
	static void SetWidgetPosition(
		UUserWidget* widget,
		const FVector2D& screenPosition);

	template<typename T>
	static T* AssignWidget(UObject* object, TSubclassOf<T> widgetClass);
};

template <typename T>
T* UWidgetUtility::AssignWidget(UObject* object, TSubclassOf<T> widgetClass)
{
	if (widgetClass && ::IsValid(object))
	{
		T* createdWidget = CreateWidget<T>(object->GetWorld(), widgetClass);
		if (createdWidget)
		{
			return createdWidget;
		}
	}

	UKismetSystemLibrary::PrintString(object, TEXT("widget don't create"), true, true, FColor::Red);
	return nullptr;
}
