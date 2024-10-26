// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../Dispose/ButtonSubject.h"
#include "../UIButtonSelectController.h"
#include "GameOverUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent) override;

public:
	void Initialized(const FName& playLevelName);

private:
	void SetEvent(const FName& playLevelName);

private:
	//UPROPERTY(meta = (BindWidget))
	//class UButtonSubject* _restartButton;

	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _exitButton;

	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _titleButton;

	UUIButtonSelectController* _buttonSelectController;
};
