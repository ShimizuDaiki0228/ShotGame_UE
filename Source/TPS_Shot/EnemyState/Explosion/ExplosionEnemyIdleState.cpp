// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyIdleState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Utility/TimeManagerUtility.h"
#include "ExplosionEnemyTrackingState.h"
#include "ExplosionEnemyPatrolState.h"
#include "../../Utility/ConstUtility.h"

void ExplosionEnemyIdleState::EnterState(AExplosionEnemyActor* enemy)
{
	UKismetSystemLibrary::PrintString(enemy, TEXT("Exter Explosion Idle State"), true, true, FColor::Red, 2.f);

	_waitTime = 0.0f;
}

void ExplosionEnemyIdleState::UpdateState(AExplosionEnemyActor* enemy)
{
	if (CheckAndAttack(enemy))
	{
		return;
	}

	CalculateElapsedTime(enemy);

}

void ExplosionEnemyIdleState::ExitState(AExplosionEnemyActor* enemy)
{
	//BaseEnemyIdleState::ExitState(enemy);

	UKismetSystemLibrary::PrintString(enemy, TEXT("Exit Base Idle State"), true, true, FColor::Green, 2.f);
}

void ExplosionEnemyIdleState::CalculateElapsedTime(AExplosionEnemyActor* enemy)
{
	float deltaTime = enemy->GetWorld()->GetDeltaSeconds();

	_waitTime += deltaTime;

	if (_waitTime >= NEXT_MOVEMENT_TIME)
	{
		enemy->ChangeState(new ExplosionEnemyPatrolState());
	}
}

bool ExplosionEnemyIdleState::CheckAndAttack(AExplosionEnemyActor* enemy)
{
	if (_waitTime < 1.0f)
	{
		return false;
	}

	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = enemy->GetTarget()->GetActorLocation();

	float distance = FVector::Dist(enemyPos, playerPos);



	if (distance <= ConstUtility::ENEMY_ATTACK_DISTANCE)
	{
		enemy->ChangeState(new ExplosionEnemyTrackingState());
		return true;
	}

	return false;
}
