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

protected:
	void BeginPlay() override;

private:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget_Playing> _playingWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UGameOverUserWidget> _gameoverWidgetClass;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HUD")
		int Score;

//他の参照
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

// バインド
private:
	void Bind();

	UFUNCTION()
	void GameOver();

	int _score;

private:
	void Initialized(ALevelManager* levelManager);
	void Reset();

	template<typename T>
	T* AssignWidget(TSubclassOf<T> assignWidgetClass);
};



