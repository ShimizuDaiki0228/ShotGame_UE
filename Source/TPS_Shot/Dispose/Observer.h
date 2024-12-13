// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subject.h"

/**
 * 
 */
template<typename... TArgs>
class TPS_SHOT_API Observer : public IIObserver<TArgs...>
{
public:
	Observer() = delete;
	Observer(TSharedPtr<Subject<TArgs...>> subject)
	: _subjectRef(subject), _callback(nullptr)
	{
		// _subjectRef->ManualAttach(TSharedPtr<Observer>(this));
		UE_LOG(LogTemp, Log, TEXT("Observer Construct"));
	}

	virtual void Subscribe(TFunction<void(TArgs...)> callback)
	{
		_callback = callback;
	}

	virtual ~Observer() override
	{
		//UKismetSystemLibrary::PrintString(this, "goodbye Observer", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	virtual void SetSharedPtr(TWeakPtr<IIObserver<TArgs...>> sharedObserver) override
	{
		_sharedObserver = sharedObserver;
		_subjectRef->ManualAttach(_sharedObserver);
	}

	virtual void RemoveObserver() override
	{
		if (_subjectRef.IsValid())
		{
			_subjectRef->Detach(_sharedObserver);
		}
		UE_LOG(LogTemp, Log, TEXT("Observer Removed"));
	}
	
	virtual void OnNext(TArgs... args) override
	{
		UE_LOG(LogTemp, Log, TEXT("CallBack"));

		if (_callback)
		{
			_callback(args...);
		}
	}


private:
	FString _message;
	TSharedPtr<Subject<TArgs...>> _subjectRef;
	TFunction<void(TArgs...)> _callback;
	TWeakPtr<IIObserver<TArgs...>> _sharedObserver;
};
