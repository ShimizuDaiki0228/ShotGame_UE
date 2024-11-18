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

	//if (_owner)
	//{
	//	_rootComponent->SetupAttachment(_owner->GetRootComponent());
	//}

	if (_particleSystem)
	{
		for (int i = 0; i < _initPoolSize; i++)
		{
			UPooledParticleSystemComponent* newComponent = CreateNewPooledObject(this);
			newComponent->Pool = this;
		}
	}

	/*TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]() {
		aaa = GetPooledObject();
		}, 1.0f, handle);*/
}



UPooledParticleSystemComponent* AParticleSystemPoolActor::GetPooledObject(AActor* owner)
{
	UPooledParticleSystemComponent* pooledObject;

	if (_pooledObjectStack.Num() == 0)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("create particle"), true, true, FColor::Red, 2.f);
		pooledObject = CreateNewPooledObject(owner);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("pop particle"), true, true, FColor::Blue, 2.f);

		pooledObject = _pooledObjectStack.Pop();

		//pooledObject->DeactivateSystem();
		//pooledObject->ResetParticles();

		// パーティクルを再度アクティブ化
	}

	//TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]() {
	//	aaa = GetPooledObject();
	//	}, 2.0f, handle);

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

UPooledParticleSystemComponent* AParticleSystemPoolActor::CreateNewPooledObject(AActor* owner)
{

	if (_particleSystem)
	{

		/*UParticleSystemComponent* newComponent = UGameplayStatics::SpawnEmitterAtLocation(
			_owner->GetWorld(),
			_particleSystem,
			FVector::ZeroVector,
			FRotator::ZeroRotator);*/

			//UPooledParticleSystemComponent* newComponent = UGameplayStatics::SpawnEmitterAtLocation(
			//	GetWorld(),
			//	_particleSystem,
			//	_owner->GetActorLocation(),  // 相対位置
			//	FRotator::ZeroRotator,
			//	true  // bAutoDestroy パラメータ（必要に応じて変更可能）
			//);


			//if (newComponent)
		{
			//UKismetSystemLibrary::PrintString(this, TEXT("particle Created"), true, true, FColor::Yellow, 2.f);

			//UPooledParticleSystemComponent* newPooledComponent = Cast<UPooledParticleSystemComponent>(newComponent);
			UPooledParticleSystemComponent* newPooledComponent = NewObject<UPooledParticleSystemComponent>(owner);

			if (newPooledComponent)
			{
				newPooledComponent->bAutoActivate = false;
				newPooledComponent->SetTemplate(_particleSystem);
				newPooledComponent->SetWorldLocation(owner->GetActorLocation());
				newPooledComponent->SetWorldRotation(FRotator::ZeroRotator);

				newPooledComponent->RegisterComponent();
				newPooledComponent->SetAutoActivate(false);
				_pooledObjectStack.Add(newPooledComponent);
				/*newPooledComponent->SetAutoActivate(false);
				newPooledComponent->bAutoActivate = false;
				newPooledComponent->RegisterComponent();*/
				return newPooledComponent;
			}
		}

	}

	UKismetSystemLibrary::PrintString(this, TEXT("particle doesn't Created"), true, true, FColor::Red, 2.f);
	return nullptr;
}

void AParticleSystemPoolActor::ActiveChange(UPooledParticleSystemComponent* pooledObjectComponent, bool isPopObject)
{
	pooledObjectComponent->SetActive(isPopObject);
}

void AParticleSystemPoolActor::Test()
{
	UKismetSystemLibrary::PrintString(this, TEXT("I Key Clicked"), true, true, FColor::Red, 2.f);



	if (_particleSystem)
	{
		{
			UPooledParticleSystemComponent* newPooledComponent = NewObject<UPooledParticleSystemComponent>(this);

			if (newPooledComponent)
			{
				newPooledComponent->bAutoActivate = true;
				newPooledComponent->SetTemplate(_particleSystem);
				newPooledComponent->SetWorldLocation(this->GetActorLocation());
				newPooledComponent->SetWorldRotation(FRotator::ZeroRotator);

				newPooledComponent->RegisterComponent();
				//newPooledComponent->SetAutoActivate(false);
				_pooledObjectStack.Add(newPooledComponent);
				/*newPooledComponent->SetAutoActivate(false);
				newPooledComponent->bAutoActivate = false;
				newPooledComponent->RegisterComponent();*/
				return;
			}
		}

	}

	UKismetSystemLibrary::PrintString(this, TEXT("particle doesn't Created"), true, true, FColor::Red, 2.f);
}
