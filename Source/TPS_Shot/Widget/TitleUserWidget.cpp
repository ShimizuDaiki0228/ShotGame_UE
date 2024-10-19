// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto world = this->GetWorld();
	_exitButton->Subscribe([this, world]()
		{
			UKismetSystemLibrary::QuitGame(world, world->GetFirstPlayerController(), EQuitPreference::Quit, false);
		});
}

void UTitleUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	_startButton->Dispose();
	_exitButton->Dispose();

	_character.Reset();
}

void UTitleUserWidget::Initialized(TWeakObjectPtr<ATPS_ShotCharacter> character, FName playLevelName)
{
	_character = character;
	
	_startButton->Subscribe([this, playLevelName]()
		{
			UGameplayStatics::OpenLevel(this, playLevelName);
		});
}