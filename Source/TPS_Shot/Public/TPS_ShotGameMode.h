// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPS_ShotCharacter.h"
#include "../Widget/Public/UserWidget_Playing.h"
#include "../Widget/Public/GameOverUserWidget.h"
#include "../Widget/Public/WidgetManager.h"
#include "../Enemy/Public/SniperEnemyActor.h"
#include "../Enemy/Public/ExplosionEnemyActor.h"
#include "SpawnVolumeActor.h"
#include "TPS_ShotGameMode.generated.h"

class AMyPlayerControllerBase;

UCLASS()
class ATPS_ShotGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPS_ShotGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	void BeginPlay() override;

private:
	// const FString GAMEOVER_WIDGET_KEY = "GAMEOVER";
	// const FString PLAYING_WIDGET_KEY = "PLAYING";

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UGameOverUserWidget> _gameoverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
	TSoftObjectPtr<UWorld> _titleLevel;

//���̎Q��
private:
	UPROPERTY()
	ATPS_ShotCharacter* _character;

	TWeakObjectPtr<UGameOverUserWidget> _gameoverWidget;

	UPROPERTY()
	ASpawnVolumeActor* _spawnVolumeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASniperEnemyActor> _sniperEnemyActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AExplosionEnemyActor> _explosionEnemyActor;

	UPROPERTY()
	AMyPlayerControllerBase* _playerController;

	int _enemyKillCount;
	
// �o�C���h
private:
	void Bind();

	UFUNCTION()
	void GameOver();

public:
	AMyPlayerControllerBase* GetPlayerController() const {return _playerController;}

private:
	void Initialized();
	void Reset();

public:
	void InCreaseEnemyKillCount();
};



