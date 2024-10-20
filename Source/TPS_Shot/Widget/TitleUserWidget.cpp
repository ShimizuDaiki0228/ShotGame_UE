// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	// UInputModeControllerの設定はコンストラクタで行う必要がある
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	_inputModeController = NewObject<UInputModeController>();
	_inputModeController->Initialized(PlayerController);

	this->SetKeyboardFocus();
}

void UTitleUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	_startButton->Dispose();
	_exitButton->Dispose();

	_character.Reset();
}

FReply UTitleUserWidget::NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent)
{
	FKey keyPressed = inKeyEvent.GetKey();

	if (_buttonSelectController->ManualKeyAction(inGeometry, inKeyEvent, keyPressed))
	{
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(inGeometry, inKeyEvent);
}

void UTitleUserWidget::SetEvent(FName playLevelName)
{
	_startButton->Subscribe([this, playLevelName]()
		{
			UGameplayStatics::OpenLevel(this, playLevelName);
		});

	auto world = this->GetWorld();
	if (_exitButton)
	{
		_exitButton->Subscribe([this, world]()
			{
				UKismetSystemLibrary::QuitGame(world, world->GetFirstPlayerController(), EQuitPreference::Quit, false);
			});
	}
}

void UTitleUserWidget::Initialized(TWeakObjectPtr<ATPS_ShotCharacter> character, FName playLevelName)
{
	_character = character;

	TArray<UButtonSubject*> selectButtons;
	selectButtons.Add(_startButton);
	selectButtons.Add(_exitButton);
	_buttonSelectController = NewObject<UUIButtonSelectController>();
	_buttonSelectController->Initialized(selectButtons);

	SetEvent(playLevelName);
}