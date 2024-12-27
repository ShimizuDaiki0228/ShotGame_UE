// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotGameMode.h"

#include "LevelManager.h"
#include "ShotCharacterPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Utility/Public/WidgetUtility.h"
#include "Utility/GameFunctionInstance.h"


ATPS_ShotGameMode::ATPS_ShotGameMode(): _character(nullptr), _spawnVolumeActor(nullptr)
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
}

void ATPS_ShotGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	_character = Cast<ATPS_ShotCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!_character)
	{
		UKismetSystemLibrary::PrintString(this, "�L�����N�^�[���擾�ł��܂���ł���", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	_spawnVolumeActor = ALevelManager::GetInstance()->GetVolumeActor();
	_playerController = UGameFunctionInstance::GetInstance()->GetPlayerController(this);
	
	Initialized();
	Bind();
	// 最初は各々が呼ぶように
	// Reset();
}

void ATPS_ShotGameMode::Bind()
{
	
}

void ATPS_ShotGameMode::GameOver()
{
	// TODO
	// ���g�̎������@��ς��Ȃ��ƃN���b�V�����Ă���
	_spawnVolumeActor->GameOver();
	
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		playerController->SetCinematicMode(true, false, false, true, true);
	}
	
	_character->GameOver();
	
	// _widgetManager->ChangeViewPort(GAMEOVER_WIDGET_KEY, PLAYING_WIDGET_KEY);

	if (!_gameoverWidget.IsValid())
	{
		// ここらへんで初期化したい
		// _gameoverWidget->Initialized(playLevelName);
		_gameoverWidget = UWidgetUtility::AssignWidget(this, _gameoverWidgetClass);
	}
	_gameoverWidget->AddToViewport();
}

void ATPS_ShotGameMode::Initialized()
{
	_spawnVolumeActor->Initialized(_explosionEnemyActor, _sniperEnemyActor, _character);
	_character->Initialized();

	const FName playLevelName = _titleLevel.IsValid() ? FName(*_titleLevel.GetAssetName()) : FName(TEXT("InvalidLevel"));
}

void ATPS_ShotGameMode::Reset()
{
	if (_character)
	{
		_character->Reset();
	}
}

void ATPS_ShotGameMode::InCreaseEnemyKillCount()
{
	_enemyKillCount++;
	_playerController->GetPlayingWidget()->UpdateScore(_enemyKillCount);
}
