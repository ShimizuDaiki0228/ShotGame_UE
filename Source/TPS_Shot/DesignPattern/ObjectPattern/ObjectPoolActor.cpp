// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolActor.h"
#include "PooledObjectActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraActor.h"
#include "TPS_Shot/Niagara/NiagaraEffect.h"

void AObjectPoolActor::ReturnToPool(UPooledObjectActorComponent* pooledObject)
{
	_pooledObjectStack.Push(pooledObject);
	ActiveChange(pooledObject->GetOwner(), false);
}

UPooledObjectActorComponent* AObjectPoolActor::GetPooledObject(const AActor* owner)
{
	UPooledObjectActorComponent* pooledObject;

	if (_pooledObjectStack.Num() == 0)
	{
		pooledObject = CreateNewPooledObject();
	}
	else
	{
		pooledObject = _pooledObjectStack.Pop();
	}

	ActiveChange(pooledObject->GetOwner(), true);
	pooledObject->GetOwner()->SetActorLocation(owner->GetActorLocation());
	return pooledObject;
}

void AObjectPoolActor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < _initPoolSize; i++)
	{
		UPooledObjectActorComponent* newObject = CreateNewPooledObject();
	}
}

UPooledObjectActorComponent* AObjectPoolActor::CreateNewPooledObject()
{
	if (_objectToPool)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			AActor* newObject = world->SpawnActor<AActor>(_objectToPool, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
			if (newObject)
			{
				// 生成するのがナイアガラエフェクトの場合は特別処理
				ANiagaraEffect* niagaraEffect = static_cast<ANiagaraEffect*>(newObject);
				if (niagaraEffect)
				{
					auto niagaraComponent = niagaraEffect->GetNiagaraComponent();
					niagaraComponent->Deactivate();
					niagaraComponent->SetAutoActivate(false);
				}
				else
				{
					ActiveChange(newObject, false);
				}
				
				UPooledObjectActorComponent* pooledObjectActorComponent = NewObject<UPooledObjectActorComponent>(newObject);
				
				if (pooledObjectActorComponent)
				{
					pooledObjectActorComponent->RegisterComponent();
					pooledObjectActorComponent->Pool = this;
					_pooledObjectStack.Add(pooledObjectActorComponent);
					return pooledObjectActorComponent;
				}
			}
		}
	}

	return nullptr;
}

void AObjectPoolActor::ActiveChange(AActor* pooledObjectActor, bool isPopObject)
{
	pooledObjectActor->SetActorHiddenInGame(!isPopObject);
	pooledObjectActor->SetActorEnableCollision(isPopObject);
	pooledObjectActor->SetActorTickEnabled(isPopObject);
}
