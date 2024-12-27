// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWidgetController.h"
#include "MyPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Utility/Public/WidgetUtility.h"
#include "Utility/GameFunctionInstance.h"

void UCharacterWidgetController::Initialized(AActor* owner)
{
	_cachedOwner = owner;
	_cachedPlayerController = UGameFunctionInstance::GetInstance()->GetPlayerController(owner);
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
FVector2D UCharacterWidgetController::SetWidgetSetting(
	 UUserWidget* userWidget,
	 float widthClampSizeMin,
	 float widthClampSizeMax,
	 float heightClampSizeMin,
	 float heightClampSizeMax,
	 const bool bIsRandomPos)
{
	if (!::IsValid(userWidget))
	{
		return FVector2D::ZeroVector;
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

		UWidgetUtility::SetWidgetScale(
			ownerScreenSize,
			_cachedOwner,
			userWidget,
			_cachedPlayerController,
			widthClampSizeMin,
			widthClampSizeMax,
			heightClampSizeMin,
			heightClampSizeMax);

		FVector2D positionOffset;
		if (bIsRandomPos)
		{
			positionOffset.X = FMath::RandRange(-ownerScreenSize.X, ownerScreenSize.X);
			positionOffset.Y = FMath::RandRange(-ownerScreenSize.Y / 2, ownerScreenSize.Y / 2);
			screenPosition += FVector2D(positionOffset.X, positionOffset.Y);
		}
		UWidgetUtility::SetWidgetPosition(userWidget, screenPosition);
		return screenPosition;
	}
	else
	{
		// ��ʊO�̏ꍇ�A�E�B�W�F�b�g���\���ɂ���i�I�v�V�����j
		userWidget->SetVisibility(ESlateVisibility::Hidden);
		return FVector2D::ZeroVector;
	}
}
