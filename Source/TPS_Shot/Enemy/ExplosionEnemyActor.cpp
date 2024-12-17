// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../EnemyState/Explosion/ExplosionEnemyIdleState.h"
#include "../EnemyState/Explosion/ExplosionEnemyTrackingState.h"
#include "../Utility/TimeManagerUtility.h"
#include "Kismet/GameplayStatics.h"


AExplosionEnemyActor::AExplosionEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosionEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->SetSimulatePhysics(true);
	_mesh->SetAngularDamping(5.0f);


	//������Ԃ�Idle�ɐݒ�
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
	_currentState->ExitState(this);

	Super::SelfDestroy();
}



void AExplosionEnemyActor::Initialized(ATPS_ShotCharacter* character)
{
	Super::Initialized(character);
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