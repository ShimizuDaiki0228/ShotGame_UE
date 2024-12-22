// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyFactory.h"
#include "ExplosionEnemyFactory.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AExplosionEnemyFactory : public ABaseEnemyFactory
{
	GENERATED_BODY()
	
public:
	virtual IIEnemyProduct* Create(const FVector& position) override;
};
