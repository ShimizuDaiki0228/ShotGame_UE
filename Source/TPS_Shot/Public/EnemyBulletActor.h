// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletActor.h"
#include "../Enemy/Public/SniperEnemyActor.h"
#include "EnemyBulletActor.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AEnemyBulletActor : public ABulletActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	ASniperEnemyActor* _sniperEnemy;

public:
	void Initialized(ASniperEnemyActor* sniperEnemy);

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
