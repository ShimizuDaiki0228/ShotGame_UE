// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/WidgetManager.h"

void UWidgetManager::ChangeViewPort(const FString& newWidgetKey, const FString& deleteWidgetKey)
{
	if (IsWidgetValidCheck(newWidgetKey) &&
		!_userWidgetDictionary[newWidgetKey]->IsInViewport())
	{
		_userWidgetDictionary[newWidgetKey]->AddToViewport();
	}
	
	if (IsWidgetValidCheck(deleteWidgetKey) &&
		_userWidgetDictionary[deleteWidgetKey]->IsInViewport())
	{
		_userWidgetDictionary[deleteWidgetKey]->RemoveFromParent();
	}
}

void UWidgetManager::RegisterWidget(const FString& key, TWeakObjectPtr<UUserWidget> value)
{
	_userWidgetDictionary.Add(key, value);
}

bool UWidgetManager::IsWidgetValidCheck(const FString& key)
{
	if (!key.IsEmpty() && _userWidgetDictionary.Contains(key) &&
		_userWidgetDictionary[key].IsValid())
	{
		return true;
	}

	return false;
}

