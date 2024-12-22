// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_GameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h"


ATitle_GameMode::ATitle_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATitle_GameMode::BeginPlay()
{
	Super::BeginPlay();

	ATPS_ShotCharacter* character = Cast<ATPS_ShotCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	_character = character;
	if (_character.IsValid())
	{
		UKismetSystemLibrary::PrintString(this, "�L�����N�^�[���擾�ł��܂���ł���", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	if (_titleWidgetClass != nullptr)
	{
		widget = CreateWidget<UUserWidget>(GetWorld(), _titleWidgetClass);
		if (widget != nullptr)
		{
			widget->AddToViewport();

			UKismetSystemLibrary::PrintString(this, "Widget Create", true, true, FColor::Cyan, 2.f, TEXT("None"));

			_titleUserWidget = Cast<UTitleUserWidget>(widget);
			if (_titleUserWidget == nullptr)
			{
				UKismetSystemLibrary::PrintString(this, "TitleWidget doesn't Create", true, true, FColor::Cyan, 2.f, TEXT("None"));
			}
		}
	}

	Initialized();
}

void ATitle_GameMode::Initialized()
{
	FName playLevelName = _playLevel.IsValid() ? FName(*_playLevel.GetAssetName()) : FName(TEXT("InvalidLevel"));
	_titleUserWidget->Initialized(_character, playLevelName);
}

void ATitle_GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	_character.Reset();
}