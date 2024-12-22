// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SniperEnemyPatrolState.h"
#include "../Public/SniperEnemyIdleState.h"
#include "Kismet/KismetSystemLibrary.h"


void SniperEnemyPatrolState::EnterState(ASniperEnemyActor* enemy)
{
	
	UKismetSystemLibrary::PrintString(enemy, TEXT("Sniper Entering Idle State"), true, true, FColor::Red, 2.f);

	_moveElapsedTime = 0.0f;

	enemy->SelectPosition();

	enemy->EndLocation = enemy->GetNextPosition()->GetActorLocation();
	enemy->StartLocation = enemy->GetActorLocation();
}

void SniperEnemyPatrolState::UpdateState(ASniperEnemyActor* enemy)
{
	_moveElapsedTime += enemy->GetWorld()->GetDeltaSeconds();
	float Alpha = FMath::Clamp(_moveElapsedTime / MOVE_DURATION, 0.0f, 1.0f);

	float EasedAlpha = FMath::InterpEaseOut(0.0f, 1.0f, Alpha, 4.0f);

	FVector NewLocation = FMath::Lerp(enemy->StartLocation, enemy->EndLocation, EasedAlpha);

	enemy->SetActorLocation(NewLocation);

	if (Alpha >= 1.0f)
	{
		enemy->ChangeState(new SniperEnemyIdleState());
	}
}

void SniperEnemyPatrolState::ExitState(ASniperEnemyActor* enemy)
{
}
