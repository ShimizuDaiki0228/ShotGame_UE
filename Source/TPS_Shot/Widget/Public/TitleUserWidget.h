// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../Public/TPS_ShotCharacter.h"
#include "../Public/InputModeController.h"
#include "../Dispose/Public/ButtonSubject.h"
#include "../Public/UIButtonSelectController.h"
#include "TitleUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Initialized(TWeakObjectPtr<ATPS_ShotCharacter> character, FName playLevelName);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent) override;

private:
	void SetEvent(FName playLevelName);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _startButton;

	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _exitButton;

	TWeakObjectPtr<ATPS_ShotCharacter> _character;

	UUIButtonSelectController* _buttonSelectController;
	TArray<UButtonSubject*> _playSelectButtons;

	int _currentButtonIndex;

	UInputModeController* _inputModeController;
};
