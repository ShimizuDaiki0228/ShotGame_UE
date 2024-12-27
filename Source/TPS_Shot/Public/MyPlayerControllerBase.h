// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerControllerBase.generated.h"

class ATPS_ShotCharacter;
class UWidgetManager;
class UUserWidget_Playing;
/**
 * 
 */
UCLASS()
class TPS_SHOT_API AMyPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerControllerBase() {}
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void CreateHUD();

private:
	// const FString GAMEOVER_WIDGET_KEY = "GAMEOVER";
	// const FString PLAYING_WIDGET_KEY = "PLAYING";

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget_Playing> _playingWidgetClass;
	
	
	
private:
	UPROPERTY()
	UUserWidget_Playing* _playingWidget;
	
	TWeakObjectPtr<UWidgetManager> _widgetManager;

	bool bIsFinishedPossessAction;

	UPROPERTY()
	ATPS_ShotCharacter* _playerCharacter;

public:
	UUserWidget_Playing* GetPlayingWidget() const {return _playingWidget;}
};
