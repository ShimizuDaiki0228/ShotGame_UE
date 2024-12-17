// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotGameMode.h"

#include "ShotCharacterPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ReactiveProperty/ReadonlyReactiveProperty.h"
#include "Utility/ConstUtility.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Utility/WidgetUtility.h"


ATPS_ShotGameMode::ATPS_ShotGameMode(): _character(nullptr), _spawnVolumeActor(nullptr),
                                        _shotCharacterPlayerState(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Game/Blueprints/ShotCharacterPlayerState_BP"));
    if (PlayerStateBPClass.Succeeded())
    {
        PlayerStateClass = PlayerStateBPClass.Class;
    }
}

void ATPS_ShotGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AShotCharacterPlayerState* ShotCharacterPlayerState = Cast<AShotCharacterPlayerState>(NewPlayer->PlayerState))
	{
		_shotCharacterPlayerState = ShotCharacterPlayerState;
		_shotCharacterPlayerState->Initialized();
	}
}

void ATPS_ShotGameMode::BeginPlay()
{
	Super::BeginPlay();

	_widgetManager = NewObject<UWidgetManager>();

	_character = Cast<ATPS_ShotCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!_character)
	{
		UKismetSystemLibrary::PrintString(this, "�L�����N�^�[���擾�ł��܂���ł���", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}
	
	_playingWidget = UWidgetUtility::AssignWidget(this, _playingWidgetClass);
	_gameoverWidget = UWidgetUtility::AssignWidget(this, _gameoverWidgetClass);
	if (_playingWidget.IsValid())
	{
		_widgetManager->RegisterWidget(PLAYING_WIDGET_KEY, _playingWidget);
		_widgetManager->ChangeViewPort(PLAYING_WIDGET_KEY);
	}
	if (_gameoverWidget.IsValid())
	{
		_widgetManager->RegisterWidget(GAMEOVER_WIDGET_KEY, _gameoverWidget);
	}

	_spawnVolumeActor = ALevelManager::GetInstance()->GetVolumeActor();
	
	Initialized();
	Bind();
	Reset();
}

void ATPS_ShotGameMode::Bind()
{
	if (_character)
	{
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
			
		if (::IsValid(_shotCharacterPlayerState) && _shotCharacterPlayerState->SharedScoreUpdateAsObserver.IsValid())
		{
			_shotCharacterPlayerState->SharedScoreUpdateAsObserver
				->Subscribe([this](int score)
				{
					_playingWidget->UpdateScore(score);
				});
			
			_shotCharacterPlayerState->TWeakScoreUpdateAsObserver.Pin()->SetSharedPtr(_shotCharacterPlayerState->TWeakScoreUpdateAsObserver);
		}
	}
}

void ATPS_ShotGameMode::GameOver()
{
	// TODO
	// ���g�̎������@��ς��Ȃ��ƃN���b�V�����Ă���
	_spawnVolumeActor->GameOver();

	_playingWidget->Gameover();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		playerController->SetCinematicMode(true, false, false, true, true);
	}
	
	_character->GameOver();
	
	_widgetManager->ChangeViewPort(GAMEOVER_WIDGET_KEY, PLAYING_WIDGET_KEY);
}

void ATPS_ShotGameMode::Initialized()
{
	_spawnVolumeActor->Initialized(_explosionEnemyActor, _sniperEnemyActor, _character);
	_character->Initialized();

	const FName playLevelName = _titleLevel.IsValid() ? FName(*_titleLevel.GetAssetName()) : FName(TEXT("InvalidLevel"));
	_gameoverWidget->Initialized(playLevelName);
}

void ATPS_ShotGameMode::Reset()
{
	_shotCharacterPlayerState->ChangeScore(0);

	if (_character)
	{
		// UKismetSystemLibrary::PrintString(this, "Character Reset", true, true, FColor::Cyan, 2.f, TEXT("None"));
		_character->Reset();
	}
}