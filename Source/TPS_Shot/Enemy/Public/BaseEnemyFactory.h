// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IEnemyProduct.h"
#include "BaseEnemyFactory.generated.h"

UCLASS(Abstract)
class TPS_SHOT_API ABaseEnemyFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemyFactory();
	virtual ~ABaseEnemyFactory() = default;

	virtual IIEnemyProduct* Create(const FVector& position) PURE_VIRTUAL(ABaseEnemyFactory::Create, return nullptr;);
};
