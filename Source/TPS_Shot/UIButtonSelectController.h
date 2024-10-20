// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dispose/ButtonSubject.h"
#include "UIButtonSelectController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UUIButtonSelectController : public UObject
{
	GENERATED_BODY()

public:
	UUIButtonSelectController();
	void Initialized(TArray<UButtonSubject*> selectButtons);

	void ButtonClick();

	virtual bool ManualKeyAction(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent,const FKey& keyPressed);

private:
	TArray<UButtonSubject*> _selectButtons;
	int _currentButtonIndex;
	
	void ChangeSelectButton(const bool isRightOrUpKey);
};
