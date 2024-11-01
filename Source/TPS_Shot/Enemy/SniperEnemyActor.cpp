// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../EnemyBulletActor.h"
#include "../Utility/TimeManagerUtility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASniperEnemyActor::ASniperEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;	


}

void ASniperEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->SetSimulatePhysics(false);

	_cacheWorld = GetWorld();
}

void ASniperEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (_beamEffectSystemInstance && GetTarget())
	{
		_beamEffectSystemInstance->SetWorldLocation(GetActorLocation());
		FVector targetLocation = GetTarget()->GetActorLocation();
		_beamEffectSystemInstance->SetVectorParameter(TEXT("Beam End"), targetLocation);
	}*/

	if (GetTarget())
	{
		//UKismetSystemLibrary::PrintString(this, TEXT("target isnt null"), true, true, FColor::Green, 2.f);
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetTarget()->GetActorLocation());
		SetActorRotation(lookAtRotation);
	}

	if (_rayObject->IsHit(this, GetTarget(), _cacheWorld))
	{
		FRotator shotRotation = GetActorRotation() + FRotator(90, 0, 0);
		_shotSpawnManager->SetTransform(GetActorLocation(), shotRotation);
		AEnemyShotActor* shotActor = _shotSpawnManager->SpawnActor(_enemyShotActorClass);
		shotActor->Initialized(GetActorLocation(), GetTarget()->GetActorLocation(), GetActorRotation());
	}

	if (_currentState)
	{
		_currentState->UpdateState(this);
	}
}

void ASniperEnemyActor::ChangeState(ISniperEnemyState* newState)
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

void ASniperEnemyActor::Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager)
{
	Super::Initialized(character, levelManager);

	SetPatrolAreaOrder();

	if (_nextPosition != nullptr)
	{
		//初期状態はIdleに設定
		_currentState = std::make_unique<SniperEnemyIdleState>();
		_currentState->EnterState(this);

		BeamShot();
	}

	if (_enemyShotActorClass)
	{
		_shotSpawnManager = NewObject<USpawnManager>();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		_shotSpawnManager->SetSpawnParameter(spawnParams);

	}

	//if (_beamEffect)
	//{
	//	FActorSpawnParameters beamEffectSpawnParams;
	//	beamEffectSpawnParams.Owner = this;

	//	AEnemyBeamEffect* beamEffect = _cacheWorld->SpawnActor<AEnemyBeamEffect>(_beamEffect, GetActorLocation(), FRotator::ZeroRotator, beamEffectSpawnParams);

	//	/*if (beamEffect)
	//	{
	//		_beamEffectSystemInstance = beamEffect->GetNiagaraComponent();

	//		if (_beamEffectSystemInstance && GetTarget())
	//		{
	//			FVector targetLocation = GetTarget()->GetActorLocation();
	//			_beamEffectSystemInstance->SetVectorParameter(TEXT("BeamEnd"), targetLocation);
	//		}
	//	}*/

	//	
	//}

	_rayObject = NewObject<URayObject>();
}

void ASniperEnemyActor::SetPatrolAreaOrder()
{
	TMap<AActor*, bool> map = GetLevelManager()->GetPatrolAreaMap();

	int8 mapSize = map.Num();

	for (int i = 0; i < mapSize; i++)
	{
		int random = FMath::RandRange(0, mapSize - 1);

		if (_alreadyAppearanceNumber.Contains(random))
		{
			while (!_alreadyAppearanceNumber.Contains(random))
			{
				random = (random + 1) % mapSize;
			}
		}

		PatrolAreaOrder.Add(random);
		_alreadyAppearanceNumber.Add(random);
	}
	
	SelectPosition();
}

void ASniperEnemyActor::SelectPosition()
{
	const TMap<AActor*, bool>& patrolAreaMap = GetLevelManager()->GetPatrolAreaMap();

	if (!patrolAreaMap.IsEmpty())
	{
		TArray<AActor*> keys;
		patrolAreaMap.GetKeys(keys);

		for (int8 i = 0; i < keys.Num(); i++)
		{
			int8 patrolAreaNumber = PatrolAreaOrder[i];

			AActor* patrolArea = keys[patrolAreaNumber];
			bool isSelected = patrolAreaMap[patrolArea];

			if (!isSelected)
			{
				GetLevelManager()->SetPatrolAreaMap(_nextPosition, patrolArea);
				_nextPosition = patrolArea;
				return;
			}
		}

		// もし移動する場所がない場合は敵の数が限界までいるため破棄する
		UKismetSystemLibrary::PrintString(this, TEXT("Position is Null, this enemy destroy"), true, true, FColor::Green, 2.f);
		SelfDestroy();
	}
}

void ASniperEnemyActor::BeamShot()
{
	if (!bCanShot) return;

	if (_beamShotSound)
	{
		SoundManagerUtility::GetInstance().Play(_beamShotSound, this);
	}
}

void ASniperEnemyActor::SetupCurrentPatrolArea()
{
}

void ASniperEnemyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetLevelManager()->SetPatrolAreaMap(nullptr, _nextPosition);

	TimeManagerUtility::GetInstance().Cancel(_cacheWorld, _createBulletTimerHandle);
}

void ASniperEnemyActor::SelfDestroy()
{

	Super::SelfDestroy();
}
