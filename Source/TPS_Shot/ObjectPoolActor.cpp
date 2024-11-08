// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolActor.h"

void AObjectPoolActor::ReturnToPool(APooledObjectActor* pooledObject)
{
	_pooledObjectStack.Push(pooledObject);
	pooledObject->SetActorHiddenInGame(true);
}
