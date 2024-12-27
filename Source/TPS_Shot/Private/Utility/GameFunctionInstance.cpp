// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/GameFunctionInstance.h"

#include "WorldPartition/ContentBundle/ContentBundleLog.h"

UGameFunctionInstance* UGameFunctionInstance::_instance = nullptr;

void UGameFunctionInstance::PostInitProperties()
{
	UObject::PostInitProperties();

	UE_LOG(LogTemp, Warning, TEXT("GameFunctionInstance PostInit"));
}

void UGameFunctionInstance::Initialized()
{
	_instance = NewObject<UGameFunctionInstance>();
	_instance->AddToRoot();
}
