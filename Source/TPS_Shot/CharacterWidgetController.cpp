// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWidgetController.h"

#include "ActorScreenSizeCalculator.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/WidgetUtility.h"

void UCharacterWidgetController::Initialized(AActor* owner, APlayerController* playerController)
{
	_cachedOwner = owner;
	_cachedPlayerController = playerController;
}

bool UCharacterWidgetController::IsCharacterProjected(FVector2D& screenPosition) const
{
	if (!::IsValid(_cachedPlayerController))
	{
		UKismetSystemLibrary::PrintString(_cachedOwner, TEXT("widget or playerController isn't valid"), true, true, FColor::Red);
		return false;
	}

	bool bProjected = UGameplayStatics::ProjectWorldToScreen(_cachedPlayerController,
	                                                    _cachedOwner->GetActorLocation() + FVector(0, 0, 100),
	                                                    screenPosition);
	return bProjected;
}

/// 
/// マルチプレイをするならTWeakObjectPtrで有効性を確認したほうがいい
/// 
void UCharacterWidgetController::SetWidgetSetting(
	 UUserWidget* userWidget,
	 float widthClampSizeMin,
	 float widthClampSizeMax,
	 float heightClampSizeMin,
	 float heightClampSizeMax,
	 const bool bIsRandomPos)
{
	if (!::IsValid(userWidget))
	{
		return;
	}
	
	FVector2D screenPosition;

	if (IsCharacterProjected(screenPosition))
	{
		if (!userWidget->IsInViewport())
		{
			userWidget->AddToViewport();
			userWidget->SetVisibility(ESlateVisibility::Visible);
		}

		FVector2D ownerScreenSize;

		UWidgetUtility::GetInstance()->SetWidgetScale(
			ownerScreenSize,
			_cachedOwner,
			userWidget,
			_cachedPlayerController,
			widthClampSizeMin,
			widthClampSizeMax,
			heightClampSizeMin,
			heightClampSizeMax);

		if (bIsRandomPos)
		{
			FVector2D positionOffset;
			positionOffset.X = FMath::RandRange(-ownerScreenSize.X, ownerScreenSize.X);
			positionOffset.Y = FMath::RandRange(-ownerScreenSize.Y / 2, ownerScreenSize.Y / 2);
			screenPosition += positionOffset;
		}
		UWidgetUtility::GetInstance()->SetWidgetPosition(userWidget, screenPosition);
	}
	else
	{
		// ��ʊO�̏ꍇ�A�E�B�W�F�b�g���\���ɂ���i�I�v�V�����j
		userWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
