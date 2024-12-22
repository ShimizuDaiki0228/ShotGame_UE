// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModeController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInputModeController::UInputModeController()
{
}

/// <summary>
/// 今のところカーソルを表示指せない予定なのでこれだけで
/// 必要が出てきた場合は条件分岐させる
/// </summary>
/// <param name="playerController"></param>
void UInputModeController::Initialized(APlayerController* playerController)
{
	if (playerController)
	{
		playerController->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		playerController->SetInputMode(InputMode);

		playerController->SetMouseCursorWidget(EMouseCursor::None, nullptr);
	}
}
