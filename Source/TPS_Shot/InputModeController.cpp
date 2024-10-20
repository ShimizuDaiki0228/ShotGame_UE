// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModeController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInputModeController::UInputModeController()
{
}

/// <summary>
/// ���̂Ƃ���J�[�\����\���w���Ȃ��\��Ȃ̂ł��ꂾ����
/// �K�v���o�Ă����ꍇ�͏������򂳂���
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
