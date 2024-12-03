// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUtility.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TPS_Shot/ActorScreenSizeCalculator.h"
#include "TPS_Shot/Widget/IFollowActorWidget.h"


UWidgetUtility* UWidgetUtility::_instance = nullptr;

UWidgetUtility* UWidgetUtility::GetInstance()
{
	if (_instance)
	{
		_instance = NewObject<UWidgetUtility>();
		// ガベージコレクションから保護
		_instance->AddToRoot();
	}
	return _instance;
}

void UWidgetUtility::SetWidgetWidthScale(const AActor* owner,
	UUserWidget* widget,
	const APlayerController* playerController,
	const float widthClampSizeMin,
	const float widthClampSizeMax,
	const float heightSize)
{
	float ownerScreenSize = UActorScreenSizeCalculator::CalculateScreenWidthSize(owner,
			playerController,
			widthClampSizeMin,
			widthClampSizeMax);

	if (ownerScreenSize != 0)
	{
		// よくない気がしている、今後要修正
		if (IIFollowActorWidget* followInterface = Cast<IIFollowActorWidget>(widget))
		{
			followInterface->SetSize(ownerScreenSize, heightSize);
		}
	}
}

void UWidgetUtility::SetWidgetPosition(UUserWidget* widget,
                                       const FVector2D screenPosition)
{
	FVector2D widgetSize = widget->GetDesiredSize();
	FVector2D centeredPosition = screenPosition - (widgetSize * 0.5f);
	widget->SetPositionInViewport(centeredPosition, true);
}