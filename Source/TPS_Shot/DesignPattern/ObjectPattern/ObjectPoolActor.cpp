// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolActor.h"
#include "PooledObjectActor.h"


void AObjectPoolActor::ReturnToPool(APooledObjectActor* pooledObject)
{
	_pooledObjectStack.Push(pooledObject);
	ActiveChange(pooledObject, false);
}

APooledObjectActor* AObjectPoolActor::GetPooledObject()
{
	APooledObjectActor* pooledObject;

	if (_pooledObjectStack.Num() == 0)
	{
		pooledObject = CreateNewPooledObject();
	}
	else
	{
		pooledObject = _pooledObjectStack.Pop();
	}

	ActiveChange(pooledObject, true);
	return pooledObject;
}

void AObjectPoolActor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < _initPoolSize; i++)
	{
		APooledObjectActor* newObject = CreateNewPooledObject();
		newObject->Pool = this;
	}
}

APooledObjectActor* AObjectPoolActor::CreateNewPooledObject()
{
	if (_objectToPool)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			APooledObjectActor* newObject = world->SpawnActor<APooledObjectActor>(_objectToPool, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
			if (newObject)
			{
				ActiveChange(newObject, false);
				newObject->Pool = this;
				_pooledObjectStack.Add(newObject);
				return newObject;
			}
		}
	}

	return nullptr;
}

void AObjectPoolActor::ActiveChange(APooledObjectActor* pooledObjectActor, bool isPopObject)
{
	pooledObjectActor->SetActorHiddenInGame(!isPopObject);
	pooledObjectActor->SetActorEnableCollision(isPopObject);
	pooledObjectActor->SetActorTickEnabled(isPopObject);
}
