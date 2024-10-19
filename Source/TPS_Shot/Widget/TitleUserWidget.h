// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../TPS_ShotCharacter.h"
#include "../Dispose/ButtonSubject.h"
#include "TitleUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void Initialized(TWeakObjectPtr<ATPS_ShotCharacter> character, FName playLevelName);
private:
	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _startButton;

	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _exitButton;

	TWeakObjectPtr<ATPS_ShotCharacter> _character;
};
