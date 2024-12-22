// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Widget/Public/TitleUserWidget.h"
#include "TPS_ShotCharacter.h"
#include "../Dispose/Public/Subject.h"
#include "Title_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ATitle_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATitle_GameMode();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> _titleWidgetClass;

	UPROPERTY()
	class UUserWidget* widget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
		TSoftObjectPtr<UWorld> _playLevel;

private:
	void Initialized();

	UTitleUserWidget* _titleUserWidget;

	TWeakObjectPtr<ATPS_ShotCharacter> _character;
};
