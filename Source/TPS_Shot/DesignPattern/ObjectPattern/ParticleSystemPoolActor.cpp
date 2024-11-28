// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleSystemPoolActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PooledParticleSystemComponent.h"
FCriticalSection PoolLock;

// Sets default values
AParticleSystemPoolActor::AParticleSystemPoolActor(): _particleSystem(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts
void AParticleSystemPoolActor::BeginPlay()
{
	Super::BeginPlay();

	if (_particleSystem)
	{
		for (int i = 0; i < _initPoolSize; i++)
		{
			TWeakObjectPtr<UPooledParticleSystemComponent> newComponent = CreateNewPooledObject();
			newComponent->Pool = this;
		}
	}
}

TWeakObjectPtr<UPooledParticleSystemComponent> AParticleSystemPoolActor::GetPooledObject(const AActor* owner)
{
	TWeakObjectPtr<UPooledParticleSystemComponent> pooledObject;
	
	if (_pooledObjectStack.Num() == 0)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Create!!!!"), true, true, FColor::Yellow, 2.f);

		pooledObject = CreateNewPooledObject();
	}
	else
	{
		pooledObject = _pooledObjectStack.Pop();
	}

	pooledObject->ActivateSystem();
	pooledObject->SetWorldLocation(owner->GetActorLocation());
	return pooledObject;
}

///GetPooledObjectとタイミングがかぶった時に_pooledObjectStack.Num() == 0の部分でクラッシュしていそう
///現状プールの中で保持しているオブジェクト数を増やすことで対処しているが今後考える必要性はある
void AParticleSystemPoolActor::ReturnToPool(TWeakObjectPtr<UPooledParticleSystemComponent> particleComponent)
{
	if (particleComponent.IsValid())
	{
		particleComponent->DeactivateSystem();

		// 同時にアクセスする場合も考えられるのでスコープ内でのみ一時的にロック
		PoolLock.Lock();
		{
			FScopeLock Lock(&PoolLock);
			_pooledObjectStack.Push(particleComponent);
		}
		PoolLock.Unlock();
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pool is Locked"), true, true, FColor::Purple);
	}
}

TWeakObjectPtr<UPooledParticleSystemComponent> AParticleSystemPoolActor::CreateNewPooledObject()
{
	if (_particleSystem)
	{
		UPooledParticleSystemComponent* newPooledComponent = NewObject<UPooledParticleSystemComponent>(this);

		if (newPooledComponent)
		{
			newPooledComponent->bAutoActivate = false;
			newPooledComponent->SetTemplate(_particleSystem);
			newPooledComponent->SetWorldLocation(FVector::ZeroVector);
			newPooledComponent->SetWorldRotation(FRotator::ZeroRotator);

			newPooledComponent->RegisterComponent();
			newPooledComponent->SetAutoActivate(false);
			_pooledObjectStack.Add(newPooledComponent);
			return newPooledComponent;
		}
	}

	UKismetSystemLibrary::PrintString(this, TEXT("particle doesn't Created"), true, true, FColor::Red, 2.f);
	return nullptr;
}