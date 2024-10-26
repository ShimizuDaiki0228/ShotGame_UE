// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolumeActor.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "LevelManager.h"
#include "Utility/TimeManagerUtility.h"


// Sets default values
ASpawnVolumeActor::ASpawnVolumeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = _whereToSpawn;

	_spawnDelay = 3.0f;
}

// Called when the game starts or when spawned
void ASpawnVolumeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolumeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnVolumeActor::SpawnEnemy(ALevelManager* levelManager)
{
	UWorld* const world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParameters;

		spawnParameters.Owner = this;
		spawnParameters.Instigator = GetInstigator();

		FVector spawnLocation = GetRandomPointInVolume();
		FRotator spawnRotation;

		spawnRotation.Yaw = FMath::FRand() * 360.0f;

		TWeakObjectPtr<AEnemyActor> spawnedEnemy = DecideGenerateEnemy(world, spawnLocation, spawnRotation, spawnParameters);
		_spawnEnemies.Add(spawnedEnemy);

		if (_character && spawnedEnemy.IsValid())
		{
			spawnedEnemy->Initialized(_character, levelManager);
		}
		TimeManagerUtility::GetInstance().Delay(world, this, &ASpawnVolumeActor::SpawnEnemy, _spawnDelay, _spawnEnemyTimerHandle, levelManager);
	}
}

TWeakObjectPtr<AEnemyActor> ASpawnVolumeActor::DecideGenerateEnemy(UWorld* world, FVector spawnLocation, FRotator spawnRotation, FActorSpawnParameters spawnParameters)
{
	int8 randomValue = FMath::RandRange(0, 1);

	AEnemyActor* spawnedEnemy = nullptr;

	spawnedEnemy = world->SpawnActor<AExplosionEnemyActor>(_explosionEnemyActor, spawnLocation, spawnRotation, spawnParameters);

	return TWeakObjectPtr<AEnemyActor>(spawnedEnemy);
	//return world->SpawnActor<ASniperEnemyActor>(_sniperEnemyActor, spawnLocation, spawnRotation, spawnParameters);

	/*if(randomValue == 0) return world->SpawnActor<ASniperEnemyActor>(_sniperEnemyActor, spawnLocation, spawnRotation, spawnParameters);
	else return world->SpawnActor<AExplosionEnemyActor>(_explosionEnemyActor, spawnLocation, spawnRotation, spawnParameters);*/
}

FVector ASpawnVolumeActor::GetRandomPointInVolume()
{
	FVector spawnOrigin = _whereToSpawn->Bounds.Origin;

	FVector spawnExtent = _whereToSpawn->Bounds.BoxExtent;
	
	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}


void ASpawnVolumeActor::Initialized(
	TSubclassOf<AExplosionEnemyActor> explosionEnemy,
	TSubclassOf<ASniperEnemyActor> sniperEnemy,
	ATPS_ShotCharacter* character,
	ALevelManager* levelManager)
{
	_explosionEnemyActor = explosionEnemy;
	_sniperEnemyActor = sniperEnemy;
	_character = character;

	SpawnEnemy(levelManager);
}

void ASpawnVolumeActor::GameOver()
{
	DestroySpawnEnemies();

	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _spawnEnemyTimerHandle);
}

void ASpawnVolumeActor::DestroySpawnEnemies()
{
	for (auto enemy : _spawnEnemies)
	{
		if (enemy.IsValid())
		{
			enemy->SelfDestroy();
		}
	}
}

