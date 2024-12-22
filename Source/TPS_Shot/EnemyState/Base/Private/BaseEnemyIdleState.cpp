// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/BaseEnemyIdleState.h"
#include "Kismet/KismetSystemLibrary.h"

void BaseEnemyIdleState::EnterState(AEnemyActor* enemy)
{
	UKismetSystemLibrary::PrintString(enemy, TEXT("Entering Base Idle State"), true, true, FColor::Green, 2.f);
}

void BaseEnemyIdleState::UpdateState(AEnemyActor* enemy)
{
	//UKismetSystemLibrary::PrintString(enemy, TEXT("Update Base Idle State"), true, true, FColor::Green, 2.f);
}

void BaseEnemyIdleState::ExitState(AEnemyActor* enemy)
{
	UKismetSystemLibrary::PrintString(enemy, TEXT("Exit Base Idle State"), true, true, FColor::Green, 2.f);
}

