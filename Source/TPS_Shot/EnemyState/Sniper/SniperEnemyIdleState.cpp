// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperEnemyIdleState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "SniperEnemyPatrolState.h"
#include "../../Utility/TimeManagerUtility.h"

void SniperEnemyIdleState::EnterState(ASniperEnemyActor* enemy)
{
	//BaseEnemyIdleState::EnterState(enemy);

	if (!enemy->bIsBeforeApprearance)
	{
		enemy->EndLocation = enemy->GetNextPosition()->GetActorLocation();
		enemy->SetActorLocation(enemy->GetNextPosition()->GetActorLocation() + FIRST_POSITION_OFFSET_Z);
		enemy->StartLocation = enemy->GetActorLocation();

		_appearElapsedTime = 0.0f;
	}
}

void SniperEnemyIdleState::UpdateState(ASniperEnemyActor* enemy)
{
	if (!enemy->bIsBeforeApprearance)
	{
		enemy->bIsBeforeApprearance = SetFirstPosition(enemy);
	}
	else
	{
		enemy->ChangeState(new SniperEnemyPatrolState());
		//FVector enemyLocation = enemy->GetActorLocation();
		//FVector targetLocation = enemy->GetTarget()->GetActorLocation();

		//// ターゲットの位置に向かう回転を計算
		//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);

		//// スナイパーキャラクターをターゲットに向ける
		//enemy->SetActorRotation(LookAtRotation);
	}
}

void SniperEnemyIdleState::ExitState(ASniperEnemyActor* enemy)
{
	enemy->bIsBeforeApprearance = true;
	UKismetSystemLibrary::PrintString(enemy, TEXT("Sniper Exit Base Idle State"), true, true, FColor::Green, 2.f);
}

bool SniperEnemyIdleState::SetFirstPosition(ASniperEnemyActor* enemy)
{
	_appearElapsedTime += enemy->GetWorld()->GetDeltaSeconds();
	float Alpha = FMath::Clamp(_appearElapsedTime / APPEAR_DURATION, 0.0f, 1.0f);

	float EasedAlpha = FMath::InterpEaseOut(0.0f, 1.0f, Alpha, 4.0f);

	FVector NewLocation = FMath::Lerp(enemy->StartLocation, enemy->EndLocation, EasedAlpha);

	enemy->SetActorLocation(NewLocation);

	if (Alpha >= 1.0f)
	{
		return true;
	}

	return false;
}
