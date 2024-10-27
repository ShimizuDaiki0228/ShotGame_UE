// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Kismet/KismetSystemLibrary.h"

USpawnManager::USpawnManager()
{
}

void USpawnManager::SetUp(const FActorSpawnParameters& spawnParameter, const FVector location, const FRotator rotation, const FVector scale)
{
	_spawnData.SpawnParameter = spawnParameter;
	_spawnData.Location = location;
	_spawnData.Rotation = rotation;
	_spawnData.Scale = scale;
}

AActor* USpawnManager::SpawnActor(TSubclassOf<AActor> actorClass)
{
	if (!actorClass)
	{
		return nullptr;
	}

	UWorld* world = _spawnData.SpawnParameter.Owner->GetWorld();
	if (world)
	{
		FTransform spawnTransform(_spawnData.Rotation, _spawnData.Location, _spawnData.Scale);

		AActor* spawnActor = world->SpawnActor<AActor>(
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
