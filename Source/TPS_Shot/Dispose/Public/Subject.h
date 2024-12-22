// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ISubject.h"

/**
 *
 */
template<typename... TArgs>
class TPS_SHOT_API Subject final : public IISubject<TArgs...>
{
public:
	// Subject(TWeakObjectPtr<AActor> owner) : _owner(owner)
	Subject()
	{

	}

	virtual ~Subject()
	{
		// UKismetSystemLibrary::PrintString(_owner.Get(), "goodbye Subject", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	virtual void ManualAttach(TWeakPtr<IIObserver<TArgs...>> observer) override
	{
		_observers.Add(observer);
	}

	virtual void Detach(TWeakPtr<IIObserver<TArgs...>> observer) override
	{
		_observers.Remove(observer);
	}	

	virtual void Notify(TArgs... args) override
	{
		// UKismetSystemLibrary::PrintString(_owner.Get(), FString::Printf(TEXT("notify %d observers"), _observers.Num()), true, true, FColor::Cyan, 2.f, TEXT("None"));
		for (auto observer : _observers)
		{
			if (TSharedPtr<IIObserver<TArgs...>> observerPtr = observer.Pin())
			{
				observerPtr->OnNext(args...);
			}
			else
			{
				// UKismetSystemLibrary::PrintString(_owner.Get(), TEXT("observer isn't valid"), true, true, FColor::Red);
			}
		}
	}

	// 若干重い気もしている
	virtual void ShutDown() override
	{
		// for (auto observer : _observers)
		// {
		// 	if (TSharedPtr<IIObserver<TArgs...>> observerPtr = observer.Pin())
		// 	{
		// 		observerPtr->RemoveObserver();
		// 	}
		// }
		_observers.Empty();
		UE_LOG(LogTemp, Log, TEXT("Subject ShutDown"));
	}

private:
	TArray<TWeakPtr<IIObserver<TArgs...>>> _observers;
};
