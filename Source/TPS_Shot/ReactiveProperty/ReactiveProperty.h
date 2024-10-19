// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
template<typename T>
class ReactiveProperty
{
private:
	T _value;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChanged, const T&);

    FOnValueChanged OnValueChanged;

    ReactiveProperty(T initialValue = 0) : _value(initialValue) {}

    T GetValue() const
    {
        return _value;
    }

    FORCEINLINE void SetValue(T newValue)
    {
        if (_value != newValue)
        {
            _value = newValue;
            OnValueChanged.Broadcast(_value);
        }
    }
};