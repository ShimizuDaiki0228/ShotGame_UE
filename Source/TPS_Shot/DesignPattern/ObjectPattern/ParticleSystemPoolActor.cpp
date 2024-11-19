// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleSystemPoolActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PooledParticleSystemComponent.h"
#include "../../Utility/TimeManagerUtility.h"

// Sets default values
AParticleSystemPoolActor::AParticleSystemPoolActor()
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
			UPooledParticleSystemComponent* newComponent = CreateNewPooledObject();
			newComponent->Pool = this;
		}
	}
}

UPooledParticleSystemComponent* AParticleSystemPoolActor::GetPooledObject(const AActor* owner)
{
	UPooledParticleSystemComponent* pooledObject;

	if (_pooledObjectStack.Num() == 0)
	{
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

void AParticleSystemPoolActor::ReturnToPool(UPooledParticleSystemComponent* particleComponent)
{
	if (particleComponent)
	{
		particleComponent->DeactivateSystem();
		_pooledObjectStack.Push(particleComponent);
	}
}

UPooledParticleSystemComponent* AParticleSystemPoolActor::CreateNewPooledObject()
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