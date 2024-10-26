// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameOverUserWidget::NativeConstruct()
{
	

	this->SetKeyboardFocus();
}

FReply UGameOverUserWidget::NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent)
{
	if (_buttonSelectController->ManualKeyAction(inGeometry, inKeyEvent))
	{
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(inGeometry, inKeyEvent);
}

void UGameOverUserWidget::Initialized(const FName& playLevelName)
{
	TArray<UButtonSubject*> selectButtons;
	//selectButtons.Add(_restartButton);
	selectButtons.Add(_exitButton);
	selectButtons.Add(_titleButton);
	_buttonSelectController = NewObject<UUIButtonSelectController>();
	_buttonSelectController->Initialized(selectButtons);

	SetEvent(playLevelName);
}

void UGameOverUserWidget::SetEvent(const FName& playLevelName)
{
	//if (_restartButton)
	//{
	//	_restartButton->Subscribe([]()
	//		{

	//		});
	//}

	auto world = this->GetWorld();
	if (_exitButton)
	{
		_exitButton->Subscribe([this, world]()
			{
				UKismetSystemLibrary::QuitGame(world, world->GetFirstPlayerController(), EQuitPreference::Quit, false);
			});
	}

	// 現時点ではクラッシュしてしまう
	if (_titleButton)
	{
		_titleButton->Subscribe([this, playLevelName]()
			{
				UGameplayStatics::OpenLevel(this, "TitleMap");
			});
	}

	bIsFocusable = true;
}
