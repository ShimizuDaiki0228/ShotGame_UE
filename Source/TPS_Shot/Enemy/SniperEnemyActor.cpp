// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../EnemyBulletActor.h"
#include "../Utility/TimeManagerUtility.h"

ASniperEnemyActor::ASniperEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;	


}

void ASniperEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->SetSimulatePhysics(false);
}

void ASniperEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		//‰Šúó‘Ô‚ÍIdle‚Éİ’è
		_currentState = std::make_unique<SniperEnemyIdleState>();
		_currentState->EnterState(this);

		CreateBulletActor();
	}

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

		// ‚à‚µˆÚ“®‚·‚éêŠ‚ª‚È‚¢ê‡‚Í“G‚Ì”‚ªŒÀŠE‚Ü‚Å‚¢‚é‚½‚ß”jŠü‚·‚é
		UKismetSystemLibrary::PrintString(this, TEXT("Position is Null, this enemy destroy"), true, true, FColor::Green, 2.f);
		SelfDestroy();
	}
}

void ASniperEnemyActor::CreateBulletActor()
{
	if (_bulletActor)
	{
		FVector location = GetActorLocation() + GetActorForwardVector() * 500.0f;
		FRotator rotation = GetActorRotation();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		AEnemyBulletActor* bullet = GetWorld()->SpawnActor<AEnemyBulletActor>(_bulletActor, location, rotation, spawnParams);

		if (bullet)
		{
			bullet->Initialized(this);
		}

		TimeManagerUtility::GetInstance().Delay(GetWorld(), this, &ASniperEnemyActor::CreateBulletActor, 1.0f, _createBulletTimerHandle);

	}
}

void ASniperEnemyActor::SetupCurrentPatrolArea()
{
}

void ASniperEnemyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetLevelManager()->SetPatrolAreaMap(nullptr, _nextPosition);

	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _createBulletTimerHandle);
}

void ASniperEnemyActor::SelfDestroy()
{

	Super::SelfDestroy();
}
