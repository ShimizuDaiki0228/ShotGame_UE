// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "WidgetManager.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UWidgetManager : public UObject
{
	GENERATED_BODY()
	
public :
	void ChangeViewPort(const FString& newWidgetKey = "", const FString& deleteWidgetKey = "");
	void RegisterWidget(const FString& key, TWeakObjectPtr<UUserWidget> value);

private:
	TMap<FString, TWeakObjectPtr<UUserWidget>> _userWidgetDictionary;

private:
	bool IsWidgetValidCheck(const FString& key);
};
