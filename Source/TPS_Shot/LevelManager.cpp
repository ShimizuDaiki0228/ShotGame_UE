// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ALevelManager::SetPatrolAreaMap(AActor* currentPatrolArea, AActor* newPatrolArea)
{
	if (currentPatrolArea != nullptr)
	{
		_patrolAreaMap[currentPatrolArea] = false;
	}

	if (newPatrolArea != nullptr)
	{
		_patrolAreaMap[newPatrolArea] = true;
	}
}
