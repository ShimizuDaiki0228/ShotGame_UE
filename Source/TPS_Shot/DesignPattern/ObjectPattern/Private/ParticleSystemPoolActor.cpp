// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ParticleSystemPoolActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Public/PooledParticleSystemComponent.h"

// Called when the game starts
void AParticleSystemPoolActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (_pooledObject != nullptr)
	{
		for (int i = 0; i < _initPoolSize; i++)
		{
			CreateNewPooledObject();
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pool object is nullptr"), true, true, FColor::Red);
	}
}

TWeakObjectPtr<UPooledParticleSystemComponent> AParticleSystemPoolActor::GetPooledObject(const AActor* owner)
{
	TWeakObjectPtr<UPooledParticleSystemComponent> pooledObject = GetPooledObjectBase<UPooledParticleSystemComponent>();
	
	if (!pooledObject.IsValid())
	{
		// UKismetSystemLibrary::PrintString(this, TEXT("new object create"), true, true, FColor::Purple);
		pooledObject = CreateNewPooledObject();
	}

	pooledObject->ActivateSystem();
	pooledObject->SetWorldLocation(owner->GetActorLocation());
	return pooledObject;
}

void AParticleSystemPoolActor::ReturnToPool(TWeakObjectPtr<UPooledParticleSystemComponent> particleComponent)
{
	if (particleComponent.IsValid())
	{
		particleComponent->DeactivateSystem();
		ReturnToPoolBase(particleComponent);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("particleComponent isn't valid"), true, true, FColor::Red);
	}
}

bool AParticleSystemPoolActor::SetParticleSystem(const TWeakObjectPtr<UPooledParticleSystemComponent>& pooledObject) const
{
	auto particleSystem = Cast<UParticleSystem>(_pooledObject);
	if (::IsValid(particleSystem))
	{
		pooledObject->SetTemplate(particleSystem);
		return true;
	}
	return false;
}

TWeakObjectPtr<UPooledParticleSystemComponent> AParticleSystemPoolActor::CreateNewPooledObject()
{
	TWeakObjectPtr<UPooledParticleSystemComponent> newPooledObject = NewObject<UPooledParticleSystemComponent>(this);
	
	if (newPooledObject.IsValid())
	{
		newPooledObject->bAutoActivate = false;
		newPooledObject->SetWorldLocation(FVector::ZeroVector);
		newPooledObject->SetWorldRotation(FRotator::ZeroRotator);
			
		newPooledObject->RegisterComponent();
		newPooledObject->SetAutoActivate(false);
		newPooledObject->Pool = this;
		if (SetParticleSystem(newPooledObject))
		{
			_pooledObjectStack.Add(newPooledObject);
			return newPooledObject;
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("poolObject can't cast to particle system"), true, true, FColor::Red, 2.f);
			return nullptr;
		}
	}

	UKismetSystemLibrary::PrintString(this, TEXT("particle doesn't Created"), true, true, FColor::Red, 2.f);
	return nullptr;
}
