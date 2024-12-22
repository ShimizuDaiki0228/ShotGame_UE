//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "ReactiveProperty.h"
//#include "ReactivePropertyInt.generated.h"
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntValueChanged, int, newValue);
//
///**
// * 
// */
//UCLASS()
//class TPS_SHOT_API UReactivePropertyInt : public UReactiveProperty
//{
//	GENERATED_BODY()
//	
//public:
//	UPROPERTY()
//	FOnIntValueChanged OnIntValueChanged;
//
//private:
//	int _value;
//
//public:
//	UReactivePropertyInt(int initialValue = 0) : _value(initialValue) {}
//
//	int GetValue() const { return _value; }
//
//	FORCEINLINE void SetValue(int newValue)
//	{
//		if (_value != newValue)
//		{
//			_value = newValue;
//			OnIntValueChanged.Broadcast(_value);
//		}
//	}
//};
