// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWidgetController.h"

#include "ActorScreenSizeCalculator.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/WidgetUtility.h"

/// 
/// マルチプレイをするならTWeakObjectPtrで有効性を確認したほうがいい
/// 
void UCharacterWidgetController::SetWidgetSetting(AActor* owner,
	UUserWidget* userWidget,
	APlayerController* playerController,
	float widthClampSizeMin,
	float widthClampSizeMax,
	float heightSize)
{
	if (!::IsValid(userWidget) || !::IsValid(playerController))
	{
		UKismetSystemLibrary::PrintString(owner, TEXT("widget or playerController isn't valid"), true, true, FColor::Red);
		return;
	}

	FVector2D screenPosition;
	bool bProjected = UGameplayStatics::ProjectWorldToScreen(playerController,
		owner->GetActorLocation() + FVector(0, 0, 100),
		screenPosition);

	if (bProjected)
	{
		if (!userWidget->IsInViewport())
		{
			userWidget->AddToViewport();
			userWidget->SetVisibility(ESlateVisibility::Visible);
		}

		UWidgetUtility::GetInstance()->SetWidgetWidthScale(owner,
			userWidget,
			playerController,
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
