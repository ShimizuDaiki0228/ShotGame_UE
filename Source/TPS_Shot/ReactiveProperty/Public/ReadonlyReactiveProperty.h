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
		// _reactiveProperty�̃����������݂���Ȃ�
		if (TSharedPtr<ReactiveProperty<T>> sharedProperty = _reactiveProperty.Pin())
		{
			// ���o�I�ɂ�_reactiveProperty�̃C�x���g�������ōw�ǂ��āA�����Actor���ōw�ǂ��Ă��炤�Ƃ�������
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
