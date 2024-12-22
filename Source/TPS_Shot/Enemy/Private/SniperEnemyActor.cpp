// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SniperEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Public/EnemyBulletActor.h"
#include "../Utility/Public/TimeManagerUtility.h"

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

	if (_isTargetLockOn)
	{
		_shotLimit -= DeltaTime;
		if (_shotLimit <= 0.0f)
		{
			int rand = FMath::RandRange(2, 4);
			BeamShot(rand);
			_shotLimit = SHOT_DURATION;
			_isTargetLockOn = false;
		}
	}

	_elapsedRayTime -= DeltaTime;

	/*if (_beamEffectSystemInstance && GetTarget())
	{
		_beamEffectSystemInstance->SetWorldLocation(GetActorLocation());
		FVector targetLocation = GetTarget()->GetActorLocation();
		_beamEffectSystemInstance->SetVectorParameter(TEXT("Beam End"), targetLocation);
	}*/

	if (GetTarget())
	{
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetTarget()->GetActorLocation());
		SetActorRotation(lookAtRotation);
	}

	if (!btest)
	{
		if (_elapsedRayTime <= 0)
		{
			_elapsedRayTime = RAY_SPAN;
			if (_rayObject->IsHit(this, GetTarget(), _cacheWorld))
			{
				_isTargetLockOn = true;
				btest = true;
			}
		}
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

void ASniperEnemyActor::Initialized(ATPS_ShotCharacter* character)
{
	Super::Initialized(character);

	_shotLimit = SHOT_DURATION;
	_elapsedRayTime = RAY_SPAN;

	SetPatrolAreaOrder();

	if (_nextPosition != nullptr)
	{
		//������Ԃ�Idle�ɐݒ�
		_currentState = std::make_unique<SniperEnemyIdleState>();
		_currentState->EnterState(this);
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
	/*TMap<AActor*, bool> map = GetLevelManager()->GetPatrolAreaMap();

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
	
	SelectPosition();*/
}

void ASniperEnemyActor::SelectPosition()
{
	const TMap<AActor*, bool>& patrolAreaMap = ALevelManager::GetInstance()->GetPatrolAreaMap();

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
				ALevelManager::GetInstance()->SetPatrolAreaMap(_nextPosition, patrolArea);
				_nextPosition = patrolArea;
				return;
			}
		}

		// �����ړ�����ꏊ���Ȃ��ꍇ�͓G�̐������E�܂ł��邽�ߔj������
		UKismetSystemLibrary::PrintString(this, TEXT("Position is Null, this enemy destroy"), true, true, FColor::Green, 2.f);
		SelfDestroy();
	}
}

void ASniperEnemyActor::BeamShot(int shotNum)
{
	shotNum--;

	if (_beamShotSound)
	{
		SoundManagerUtility::GetInstance().Play(_beamShotSound, this);
	}

	FRotator shotRotation = GetActorRotation() + SHOT_ROTATION_OFFSET;
	_shotSpawnManager->SetTransform(GetActorLocation(), shotRotation);
	AEnemyShotActor* shotActor = _shotSpawnManager->SpawnActor(_enemyShotActorClass);
	FVector targetLocation = GetTarget()->GetActorLocation() + TARGET_OFFSET;
	shotActor->Initialized(GetActorLocation(), targetLocation, GetActorRotation());

	if (shotNum > 0)
	{
		TimeManagerUtility::GetInstance().Delay(_cacheWorld, [this, shotNum]()
			{
				BeamShot(shotNum);
			}, 0.1f, _shotTimeHandle);
	}
}

void ASniperEnemyActor::SetupCurrentPatrolArea()
{
}

void ASniperEnemyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ALevelManager::GetInstance()->SetPatrolAreaMap(nullptr, _nextPosition);

	TimeManagerUtility::GetInstance().Cancel(_cacheWorld, _shotTimeHandle);
}

void ASniperEnemyActor::SelfDestroy()
{

	Super::SelfDestroy();
}
