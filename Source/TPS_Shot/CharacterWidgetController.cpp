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

bool UCharacterWidgetController::IsCharacterProjected(FVector2D& screenPosition)
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
	float heightSize)
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

		UWidgetUtility::GetInstance()->SetWidgetWidthScale(_cachedOwner,
			userWidget,
			_cachedPlayerController,
			widthClampSizeMin,
			widthClampSizeMax,
			heightSize);

		UWidgetUtility::GetInstance()->SetWidgetPosition(userWidget, screenPosition);
	}
	else
	{
		// ��ʊO�̏ꍇ�A�E�B�W�F�b�g���\���ɂ���i�I�v�V�����j
		userWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterWidgetController::SetTextSetting(UUserWidget* text,
	const float& widthClampSizeMin,
	const float& widthClampSizeMax,
	const bool bIsRandomPos)
{
	if (!::IsValid(text))
	{
		return;
	}

	FVector2D screenPosition;
	
	if (IsCharacterProjected(screenPosition))
	{
		
		FVector2D positionOffset = FVector2D::ZeroVector;
		if (bIsRandomPos)
		{
			// TODO
			// 自身の描画されているサイズの大きさ+-の幅のほうがよさそう
			positionOffset = FVector2D(FMath::FRandRange(-TEXT_POSITION_OFFSET_X, TEXT_POSITION_OFFSET_X),
								       FMath::FRandRange(-TEXT_POSITION_OFFSET_Y, TEXT_POSITION_OFFSET_Y));
		}
		UWidgetUtility::GetInstance()->SetWidgetPosition(text, screenPosition, positionOffset);
	}
}
