// Fill out your copyright notice in the Description page of Project Settings.


#include "UIButtonSelectController.h"

UUIButtonSelectController::UUIButtonSelectController()
{
	_currentButtonIndex = 0;
}

void UUIButtonSelectController::Initialized(TArray<UButtonSubject*> selectButtons)
{
	_selectButtons = selectButtons;

	for (auto button : _selectButtons)
	{
		if (button)
		{
			// 最初のボタンのみフォーカスさせる
			button->SetKeyboardFocus();
			break;
		}
	}
}

void UUIButtonSelectController::ChangeSelectButton(const bool isRightOrUpKey)
{
	int amountOfChange = isRightOrUpKey ? 1 : -1;
	_currentButtonIndex = (_currentButtonIndex + amountOfChange) % _selectButtons.Num();

	if (_selectButtons[_currentButtonIndex])
	{
		_selectButtons[_currentButtonIndex]->SetKeyboardFocus();
	}
}

void UUIButtonSelectController::ButtonClick()
{
	if (_selectButtons.IsValidIndex(_currentButtonIndex) && _selectButtons[_currentButtonIndex])
	{
		_selectButtons[_currentButtonIndex]->HandleOnClick();
	}
}

bool UUIButtonSelectController::ManualKeyAction(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent,const FKey& keyPressed)
{
	if (_selectButtons.Num() == 0)
	{
		return false;
	}

	if (keyPressed == EKeys::Right)
	{
		ChangeSelectButton(true);
		return true;
	}
	else if (keyPressed == EKeys::Left)
	{
		ChangeSelectButton(false);
		return true;
	}

	if (keyPressed == EKeys::SpaceBar)
	{
		ButtonClick();
		return true;
	}

	return false;
}
