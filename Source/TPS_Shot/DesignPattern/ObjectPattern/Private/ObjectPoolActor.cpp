// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ObjectPoolActor.h"
#include "../Public/PooledObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraActor.h"
#include "TPS_Shot/Niagara/Public/NiagaraEffect.h"
#include "NiagaraComponent.h"

void AObjectPoolActor::ReturnToPool(AActor* pooledObject)
{
	// 少しDelayをかけて成功するか試してもいいかも
	if (pooledObject)
	{
		_pooledObjectStack.Add(pooledObject);
		ActiveChange(pooledObject, false);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pooledObject isn't valid"), true, true, FColor::Red);
	}
}

// AActor* AObjectPoolActor::GetPooledObject(const AActor* owner)
// {
// 	APooledObject* pooledObject;
//
// 	if (_pooledObjectStack.Num() == 0)
// 	{
// 		pooledObject = CreateNewPooledObject();
// 	}
// 	else
// 	{
// 		pooledObject = _pooledObjectStack.Pop();
// 	}
//
// 	ActiveChange(pooledObject->GetOwner(), true);
// 	pooledObject->GetOwner()->SetActorLocation(owner->GetActorLocation());
// 	return pooledObject;
// }

void AObjectPoolActor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < _initPoolSize; i++)
	{
		AActor* newObject = CreateNewPooledObject();
	}
}

AActor* AObjectPoolActor::CreateNewPooledObject()
{
	if (_objectToPool)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			// FActorSpawnParameters spawnParams;
			// spawnParams.Owner = this;
			AActor* pooledObject = world->SpawnActor<AActor>(_objectToPool, FVector::ZeroVector, FRotator::ZeroRotator);
			if (pooledObject)
			{
				if (auto iPooledObject = Cast<IIPooledObject>(pooledObject))
				{
					iPooledObject->Pool = this;
					_pooledObjectStack.Add(pooledObject);
				}
				
				//生成するのがナイアガラエフェクトの場合は特別処理
				ANiagaraEffect* niagaraEffect = Cast<ANiagaraEffect>(pooledObject);
				if (niagaraEffect)
				{
					UKismetSystemLibrary::PrintString(this, TEXT("niagara"), true, true, FColor::Purple);
					auto niagaraComponent = niagaraEffect->GetNiagaraComponent();
					niagaraComponent->Deactivate();
					niagaraComponent->SetAutoActivate(false);
				}
				else
				{
					UKismetSystemLibrary::PrintString(this, TEXT("not niagara"), true, true, FColor::Yellow);
					ActiveChange(pooledObject, false);
				}
				
				return pooledObject;
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
