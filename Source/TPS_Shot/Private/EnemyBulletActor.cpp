// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBulletActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TPS_ShotCharacter.h"

void AEnemyBulletActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBulletActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	ATPS_ShotCharacter* character = Cast<ATPS_ShotCharacter>(OtherActor);
}

void AEnemyBulletActor::Initialized(ASniperEnemyActor* sniperEnemy)
{
	_sniperEnemy = sniperEnemy;
}
