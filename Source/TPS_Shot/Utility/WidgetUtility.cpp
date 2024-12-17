// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUtility.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TPS_Shot/ActorScreenSizeCalculator.h"
#include "TPS_Shot/Widget/IFollowActorWidget.h"



void UWidgetUtility::SetWidgetScale(
	FVector2D& ownerScreenSize,
	const AActor* owner,
	UUserWidget* widget,
	const APlayerController* playerController,
	const float widthClampSizeMin,
	const float widthClampSizeMax,
	const float heightClampSizeMin,
	const float heightClampSizeMax)
{
	ownerScreenSize = UActorScreenSizeCalculator::CalculateScreenSize(
			owner,
			playerController);

	if (ownerScreenSize != FVector2D::ZeroVector)
	{
		if (widthClampSizeMax != 0)
		{
			ownerScreenSize.X = FMath::Clamp(ownerScreenSize.X, widthClampSizeMin, widthClampSizeMax);
		}
		if (heightClampSizeMax != 0)
		{
			ownerScreenSize.Y = FMath::Clamp(ownerScreenSize.Y, heightClampSizeMin, heightClampSizeMax);
		}
		
		// よくない気がしている、今後要修正
		if (IIFollowActorWidget* followInterface = Cast<IIFollowActorWidget>(widget))
		{
			followInterface->SetSize(ownerScreenSize);

		}
	}
}

void UWidgetUtility::SetWidgetPosition(UUserWidget* widget, const FVector2D& screenPosition)
{
	FVector2D widgetSize = widget->GetDesiredSize();
	FVector2D definitePosition = screenPosition - (widgetSize * 0.5f);
	widget->SetPositionInViewport(definitePosition, true);
}
