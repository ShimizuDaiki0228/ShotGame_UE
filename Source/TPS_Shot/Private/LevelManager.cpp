// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "Kismet/GameplayStatics.h"

ALevelManager* ALevelManager::_instance = nullptr;

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

ALevelManager* ALevelManager::GetInstance()
{
	// シングルトンクラス作成の続き
	if (_instance == nullptr)
	{
		// // レベル上にある LevelManager を検索
		// TArray<AActor*> foundActors;
		// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelManager::StaticClass(), foundActors);
		//
		// if (foundActors.Num() > 0)
		// {
		// 	_instance = Cast<ALevelManager>(foundActors[0]);
		// }
		//
		// if (_instance == nullptr)
		// {
		// 	UKismetSystemLibrary::PrintString(this, TEXT("LevelManager isn't found"), true, true, FColor::Red);
		// }
		
		// 何かしらの処理
	}
	return _instance;
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	_instance = this;
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
