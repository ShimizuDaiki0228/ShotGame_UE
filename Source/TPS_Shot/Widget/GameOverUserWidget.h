// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../Dispose/ButtonSubject.h"
#include "GameOverUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_SHOT_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	//virtual void NativeDestrucst() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _restartButton;

	UPROPERTY(meta = (BindWidget))
	class UButtonSubject* _titleButton;
};
