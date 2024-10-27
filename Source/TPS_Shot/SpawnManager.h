// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnManager.generated.h"

USTRUCT()
struct FSpawnData
{
	GENERATED_BODY()

	FActorSpawnParameters SpawnParameter;
	FVector Location;
	FRotator Rotation;
	FVector Scale;
};

UCLASS()
class TPS_SHOT_API USpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	USpawnManager();

	void SetUp(const FActorSpawnParameters& spawnParameter,
		const FVector location = FVector::ZeroVector,
		const FRotator rotation = FRotator::ZeroRotator,
		const FVector scale = FVector::OneVector);

	AActor* SpawnActor(TSubclassOf<AActor> actorClass);

private:
	FSpawnData _spawnData;
};
