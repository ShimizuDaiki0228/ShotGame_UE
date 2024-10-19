// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subject.h"

/**
 * 
 */
template<typename... TArgs>
class TPS_SHOT_API Observer : public IIObserver
{
public:
	Observer(Subject& subject, TFunction<void(TArgs...)> callback = nullptr) : _subjectRef(subject), _callback(callback)
	{
		_subjectRef.Attach(this);
		//UKismetSystemLibrary::PrintString(subject., "attach Observer", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	virtual ~Observer()
	{
		//UKismetSystemLibrary::PrintString(this, "goodbye Observer", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

	void OnNext() override
	{
		if (_callback)
		{
			_callback();
		}
	}

	void RemoveObserver()
	{
		_subjectRef.Detach(this);
		//UKismetSystemLibrary::PrintString(this, "Observer removed from list", true, true, FColor::Cyan, 2.f, TEXT("None"));
	}

private:
	FString _message;
	Subject& _subjectRef;
	TFunction<void(TArgs...)> _callback;
};
