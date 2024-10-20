// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../InputModeController.h"
#include "Kismet/GameplayStatics.h"

void UTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

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
	
	if (keyPressed == EKeys::K)
	{
		FocusNextButton();
		return FReply::Handled();
	}

	if (keyPressed == EKeys::J)
	{
		if (_playSelectButtons.IsValidIndex(_currentButtonIndex) && _playSelectButtons[_currentButtonIndex])
		{
			_playSelectButtons[_currentButtonIndex]->HandleOnClick();
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown(inGeometry, inKeyEvent);
}

void UTitleUserWidget::FocusNextButton()
{
	if (_playSelectButtons.Num() == 0) return;

	_currentButtonIndex = (_currentButtonIndex + 1) % _playSelectButtons.Num();

	if (_playSelectButtons[_currentButtonIndex])
	{
		_playSelectButtons[_currentButtonIndex]->SetKeyboardFocus();
	}
}

void UTitleUserWidget::Initialized(TWeakObjectPtr<ATPS_ShotCharacter> character, FName playLevelName)
{
	_character = character;
	
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

		_exitButton->SetKeyboardFocus();
	}

	_playSelectButtons.Add(_startButton);
	_playSelectButtons.Add(_exitButton);
}