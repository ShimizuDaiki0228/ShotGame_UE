// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ISubject.h"

/**
 *
 */
class TPS_SHOT_API Subject final : public IISubject
{
public:
	Subject(TWeakObjectPtr<AActor> owner) : _owner(owner)
	{

	}

	virtual ~Subject()
	{
		UKismetSystemLibrary::PrintString(_owner.Get(), "goodbye Subject", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	void Attach(IIObserver* observer) override
	{
		_observers.Add(observer);
	}

	void Detach(IIObserver* observer) override
	{
		_observers.Remove(observer);
	}	

	void Notify() override
	{
		UKismetSystemLibrary::PrintString(_owner.Get(), FString::Printf(TEXT("notify %d observers"), _observers.Num()), true, true, FColor::Cyan, 2.f, TEXT("None"));
		for (auto observer : _observers)
		{
			observer->OnNext();
		}
	}

private:
	TWeakObjectPtr<AActor> _owner;
	TArray<IIObserver*> _observers;
};
