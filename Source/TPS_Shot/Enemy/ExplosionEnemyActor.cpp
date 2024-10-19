// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../EnemyState/Explosion/ExplosionEnemyIdleState.h"
#include "../EnemyState/Explosion/ExplosionEnemyTrackingState.h"
#include "../Utility/TimeManagerUtility.h"
#include "../Utility/SoundManagerUtility.h"
#include "Kismet/GameplayStatics.h"


AExplosionEnemyActor::AExplosionEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosionEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->SetSimulatePhysics(true);

	//‰Šúó‘Ô‚ÍIdle‚Éİ’è
	_currentState = std::make_unique<ExplosionEnemyIdleState>();
	_currentState->EnterState(this);
}

void AExplosionEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_currentState)
	{
		_currentState->UpdateState(this);
	}
}

void AExplosionEnemyActor::SelfDestroy()
{
	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _destroyTimerHandle);

	_currentState->ExitState(this);

	Super::SelfDestroy();
}



void AExplosionEnemyActor::Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager)
{
	Super::Initialized(character, levelManager);
}

void AExplosionEnemyActor::ChangeState(IExplosionEnemyState* newState)
{
	if (_currentState.get() == newState)
	{
		return;
	}

	if (_currentState)
	{
		_currentState->ExitState(this);
	}

	_currentState.reset(newState);

	if (_currentState)
	{
		_currentState->EnterState(this);
	}
}

void AExplosionEnemyActor::Explosion()
{
	if (_explosionEffect)
	{
		if (_soundToPlay)
		{
			SoundManagerUtility::GetInstance().Play(_soundToPlay, this);
		}

		AExplosionEffect* spawnedExplosionEffect = SpawnExplosionEffect();

		if (spawnedExplosionEffect)
		{
			spawnedExplosionEffect->Initialized(GetTarget());
			TimeManagerUtility::GetInstance().Delay(GetWorld(), [this, spawnedExplosionEffect]()
				{
					spawnedExplosionEffect->Destroy();
				}, 3.0f, _destroyTimerHandle);
		}

		SelfDestroy();
	}
}

AExplosionEffect* AExplosionEnemyActor::SpawnExplosionEffect()
{
	FActorSpawnParameters spawnPrams;

	spawnPrams.Owner = this;
	spawnPrams.Instigator = GetInstigator();

	FVector spawnLocation = GetActorLocation();

	return GetWorld()->SpawnActor<AExplosionEffect>(
		_explosionEffect,
		spawnLocation,
		FRotator::ZeroRotator,
		spawnPrams);
}
