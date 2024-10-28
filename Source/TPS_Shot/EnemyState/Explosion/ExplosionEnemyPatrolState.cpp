// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyPatrolState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ExplosionEnemyTrackingState.h"
#include "../../Utility/ConstUtility.h"
#include <future>

using namespace std;

void ExplosionEnemyPatrolState::EnterState(AExplosionEnemyActor* enemy)
{
	_moveDirection = FMath::RandRange(0.0f, 360.0f);

	FRotator currentEnemyRotation = enemy->GetActorRotation();
	float newYaw = currentEnemyRotation.Yaw + _moveDirection;

	FRotator newRotator(0.0f, newYaw, 0.0f);
	FVector newDirection = newRotator.Vector();

	//// 進行方向に力を加える
	FVector Force = newDirection * 300000.0f;
	enemy->GetMesh()->AddImpulse(Force);
}

// TrackingStateでは数フレーム後にブレーキを掛けるようにしていたが、少し違うアプローチにする
void ExplosionEnemyPatrolState::UpdateState(AExplosionEnemyActor* enemy)
{
	if (bIsTransitioning)
	{
		TransitionToTracking(enemy);
	}
	else
	{
		bIsTransitioning = CheckAndAttack(enemy);
	}
}

void ExplosionEnemyPatrolState::ExitState(AExplosionEnemyActor* enemy)
{
}

void ExplosionEnemyPatrolState::TransitionToTracking(AExplosionEnemyActor* enemy)
{
	SetVelocity(0.5f, enemy);
	if (enemy->GetMesh()->GetPhysicsLinearVelocity().Size() < 100.0f)
	{
		enemy->ChangeState(new ExplosionEnemyTrackingState());
	}
}

void ExplosionEnemyPatrolState::SetVelocity(float decelerationRate, AExplosionEnemyActor* enemy)
{
	FVector currentVelocity = enemy->GetMesh()->GetPhysicsLinearVelocity();

	FVector newVelocity = currentVelocity * decelerationRate;
	enemy->GetMesh()->SetPhysicsLinearVelocity(newVelocity);
}

bool ExplosionEnemyPatrolState::CheckAndAttack(AExplosionEnemyActor* enemy)
{
	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = enemy->GetTarget()->GetActorLocation();

	float distance = FVector::Dist(enemyPos, playerPos);

	if (distance <= ConstUtility::ENEMY_ATTACK_DISTANCE)
	{
		return true;
	}

	return false;
}
