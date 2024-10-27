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

void USpawnManager::SetUp(const FActorSpawnParameters& spawnParameter, const FSpawnTransform& spawnTransform)
{
	_spawnData.SpawnParameter = spawnParameter;
	_spawnData.Location = spawnTransform.Location;
	_spawnData.Rotation = spawnTransform.Rotation;
	_spawnData.Scale = spawnTransform.Scale;
}