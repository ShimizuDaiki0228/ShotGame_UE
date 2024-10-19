// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactiveProperty.h"


/**
 * 
 */
template<typename T>
class ReadOnlyReactiveProperty
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChanged, const T&);

private:
	TWeakPtr<ReactiveProperty<T>> _reactiveProperty;

public:
	ReadOnlyReactiveProperty(TSharedPtr<ReactiveProperty<T>> reactiveProperty)
		: _reactiveProperty(reactiveProperty)
	{
		// _reactivePropertyのメモリが存在するなら
		if (TSharedPtr<ReactiveProperty<T>> sharedProperty = _reactiveProperty.Pin())
		{
			// 感覚的には_reactivePropertyのイベントをここで購読して、さらにActor側で購読してもらうという感じ
			sharedProperty->OnValueChanged.AddLambda([this](const T& newValue)
				{
					OnValueChanged.Broadcast(newValue);
				});
		}
	}


	FOnValueChanged OnValueChanged;

	T GetValue() const
	{
		if (TSharedPtr<ReactiveProperty<T>> sharedProperty = _reactiveProperty.Pin())
		{
			return sharedProperty->GetValue();
		}
		return T();
	}
};
