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
	
	_playingWidget = AssignWidget<UUserWidget_Playing>(_playingWidgetClass);
	_gameoverWidget = AssignWidget<UGameOverUserWidget>(_gameoverWidgetClass);
	if (_playingWidget)
	if (_playingWidget.IsValid())
	{
		_widgetManager->ChangeViewPort(_playingWidget.Get());
	}

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelManager::StaticClass(), foundActors);
	ALevelManager* levelManager = nullptr;

	if (foundActors.Num() > 0)
	{
		levelManager = Cast<ALevelManager>(foundActors[0]);
		if (levelManager)
		{
			_spawnVolumeActor = levelManager->GetVolumeActor();
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, "levelManager isn't Found", true, true, FColor::Cyan, 2.f, TEXT("None"));		
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

	_character->GetMesh()->SetSimulatePhysics(true);
	_character->GetMovementComponent()->MovementState.bCanJump = false;

	if (_gameoverWidget != nullptr)
	{
		_widgetManager->ChangeViewPort(_gameoverWidget.Get(), _playingWidget.Get());
	}
}

void ATPS_ShotGameMode::Initialized(ALevelManager* levelManager)
{
	_spawnVolumeActor->Initialized(_explosionEnemyActor, _sniperEnemyActor, _character, levelManager);
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

template<typename T>
T* ATPS_ShotGameMode::AssignWidget(TSubclassOf<T> assignWidgetClass)
{
    if (assignWidgetClass)
    {
        // �E�B�W�F�b�g�𐶐�
        T* assignWidget = CreateWidget<T>(GetWorld(), assignWidgetClass);
        if (assignWidget)
        {
            return assignWidget;
        }
    }

    return nullptr;
}