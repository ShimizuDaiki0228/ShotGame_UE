// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerControllerBase.h"

#include "TPS_ShotCharacter.h"
#include "TPS_Shot/Utility/Public/WidgetUtility.h"
#include "TPS_Shot/Widget/Public/UserWidget_Playing.h"
#include "TPS_Shot/Widget/Public/WidgetManager.h"

void AMyPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	_widgetManager = NewObject<UWidgetManager>();
	
	// 今は実装できていないが、将来的に任せたい
	// if (_playingWidget.IsValid())
	// {
	// 	_widgetManager->RegisterWidget(PLAYING_WIDGET_KEY, _playingWidget);
	// 	_widgetManager->ChangeViewPort(PLAYING_WIDGET_KEY);
	// }
	// if (_gameoverWidget.IsValid())
	// {
	// 	_widgetManager->RegisterWidget(GAMEOVER_WIDGET_KEY, _gameoverWidget);
	// }

	CreateHUD();
}

void AMyPlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	_playerCharacter = Cast<ATPS_ShotCharacter>(InPawn);
	if (!_playerCharacter)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("PlayerCharacter don't cast"), true, true, FColor::Red);
	}
}

///
/// 今後要修正
/// 
void AMyPlayerControllerBase::CreateHUD()
{
	if (!::IsValid(_playingWidget))
	{
		_playingWidget = UWidgetUtility::AssignWidget(this, _playingWidgetClass);
		_playingWidget->AddToViewport();
	}
	else
	{
		_playingWidget->AddToViewport();
	}
	_playingWidget->SetVisibility(ESlateVisibility::Visible);
}
