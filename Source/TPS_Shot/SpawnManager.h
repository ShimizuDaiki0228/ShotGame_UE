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

USTRUCT()
struct FSpawnTransform
{
	GENERATED_BODY()

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

	void SetUp(const FActorSpawnParameters& spawnParameter,
		const FSpawnTransform& spawnTransform);

	template<typename T>
	T* SpawnActor(TSubclassOf<T> actorClass)
	{
		if (!actorClass)
		{
			return nullptr;
		}

		UWorld* world = _spawnData.SpawnParameter.Owner->GetWorld();
		if (world)
		{
			FTransform spawnTransform(_spawnData.Rotation, _spawnData.Location, _spawnData.Scale);

			T* spawnActor = world->SpawnActor<T>(
				actorClass,
				spawnTransform,
				_spawnData.SpawnParameter);

			if (spawnActor)
			{
				return spawnActor;
			}
		}

		return nullptr;
	}

private:
	FSpawnData _spawnData;
};
