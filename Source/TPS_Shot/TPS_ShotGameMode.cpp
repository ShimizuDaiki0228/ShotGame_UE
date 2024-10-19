// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ReactiveProperty/ReadonlyReactiveProperty.h"
#include "Utility/ConstUtility.h"
#include "GameFramework/PawnMovementComponent.h"


ATPS_ShotGameMode::ATPS_ShotGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATPS_ShotGameMode::BeginPlay()
{
	Super::BeginPlay();

	_widgetManager = NewObject<UWidgetManager>();

	_character = Cast<ATPS_ShotCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!_character)
	{
		UKismetSystemLibrary::PrintString(this, "キャラクターを取得できませんでした", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}
	
	_playingWidget = AssignWidget<UUserWidget_Playing>(_playingWidgetClass);
	_gameoverWidget = AssignWidget<UGameOverUserWidget>(_gameoverWidgetClass);
	if (_playingWidget)
	{
		_widgetManager->ChangeViewPort(_playingWidget);
	}

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelManager::StaticClass(), foundActors);
	ALevelManager* levelManager = nullptr;

	if (foundActors.Num() > 0)
	{
		levelManager = Cast<ALevelManager>(foundActors[0]);
		if (levelManager)
		{
			UKismetSystemLibrary::PrintString(this, "levelManager is Found", true, true, FColor::Cyan, 2.f, TEXT("None"));		
			_spawnVolumeActor = levelManager->GetVolumeActor();
		}
	}

	
	Initialized(levelManager);
	Bind();
	Reset();
}

void ATPS_ShotGameMode::Bind()
{
	if (_character)
	{
		if (_character->ScoreProp.IsValid())
		{
			_character->ScoreProp->OnValueChanged.AddLambda([this](const int& newValue)
			{
				_score++;
				_playingWidget->UpdateScore(_score);
			});
		}

		if (_character->NumberOfBulletProp.IsValid())
		{
			_character->NumberOfBulletProp->OnValueChanged.AddLambda([this](const int& newValue)
				{
					_playingWidget->ChangeBulletNumber(newValue);
				});
		}

		if (_character->CurrentHPProp.IsValid())
		{
			_character->CurrentHPProp->OnValueChanged.AddLambda([this](const int& newValue)
			{
				float currentHpPercentage = 
					(float)_character->CurrentHPProp->GetValue() / (float)ConstUtility::PLAYER_MAX_HP;

				_playingWidget->UpdateHPBar(currentHpPercentage);

				if (newValue <= 0)
				{
					GameOver();
				}
			});
		}
	}
}

void ATPS_ShotGameMode::GameOver()
{
	_spawnVolumeActor->GameOver();

	_playingWidget->Gameover();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		playerController->SetCinematicMode(true, false, false, true, true);
	}

	_character->GetMesh()->SetSimulatePhysics(true);
	_character->GetMovementComponent()->MovementState.bCanJump = false;

	if (_gameoverWidget != nullptr)
	{
		_widgetManager->ChangeViewPort(_gameoverWidget, _playingWidget);
	}
}

void ATPS_ShotGameMode::Initialized(ALevelManager* levelManager)
{
	_spawnVolumeActor->Initialized(_explosionEnemyActor, _sniperEnemyActor, _character, levelManager);
	_character->Initialized();
}

void ATPS_ShotGameMode::Reset()
{
	_score = 0;

	if (_playingWidget)
	{
		UKismetSystemLibrary::PrintString(this, "Score Reset", true, true, FColor::Cyan, 2.f, TEXT("None"));
		_playingWidget->Reset(_score);

	}
	if (_character)
	{
		UKismetSystemLibrary::PrintString(this, "Character Reset", true, true, FColor::Cyan, 2.f, TEXT("None"));
		_character->Reset();
	}
}

template<typename T>
T* ATPS_ShotGameMode::AssignWidget(TSubclassOf<T> assignWidgetClass)
{
    if (assignWidgetClass)
    {
        // ウィジェットを生成
        T* assignWidget = CreateWidget<T>(GetWorld(), assignWidgetClass);
        if (assignWidget)
        {
            return assignWidget;
        }
    }

    return nullptr;
}