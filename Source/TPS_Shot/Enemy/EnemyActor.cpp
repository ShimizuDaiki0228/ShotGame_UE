// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyActor::SelfDestroy()
{
	Destroy();
}

void AEnemyActor::Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager)
{
	_character = character;
	_levelManager = levelManager;

	_hp = MAX_HP;
}

bool AEnemyActor::DecreaseHP()
{
	_hp--;

	if (_hp <= 0)
	{
		return true;
	}
	return false;
}
