// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPS_ShotCharacter.h"
#include "Widget/UserWidget_Playing.h"
#include "Widget/GameOverUserWidget.h"
#include "Widget/WidgetManager.h"
#include "Enemy/SniperEnemyActor.h"
#include "Enemy/ExplosionEnemyActor.h"
#include "SpawnVolumeActor.h"
#include "LevelManager.h"
#include "ReactiveProperty/ReactiveProperty.h"
#include "ReactiveProperty/ReadonlyReactiveProperty.h"
#include "TPS_ShotGameMode.generated.h"

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget_Playing> _playingWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UGameOverUserWidget> _gameoverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
		TSoftObjectPtr<UWorld> _titleLevel;

//���̎Q��
private:
	ATPS_ShotCharacter* _character;

	UUserWidget_Playing* _playingWidget;
	UGameOverUserWidget* _gameoverWidget;

	ASpawnVolumeActor* _spawnVolumeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASniperEnemyActor> _sniperEnemyActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AExplosionEnemyActor> _explosionEnemyActor;

	TObjectPtr<UWidgetManager> _widgetManager;

	UPROPERTY()
	AShotCharacterPlayerState* _shotCharacterPlayerState;
	
// �o�C���h
private:
	void Bind();

	UFUNCTION()
	void GameOver();

public:
	AShotCharacterPlayerState* GetPlayerState() const {return _shotCharacterPlayerState;}

private:
	void Initialized(ALevelManager* levelManager);
	void Reset();

	template<typename T>
	T* AssignWidget(TSubclassOf<T> assignWidgetClass);
};



