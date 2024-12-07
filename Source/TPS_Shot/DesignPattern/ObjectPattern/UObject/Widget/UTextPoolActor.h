// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UObjectPooledSystemBase.h"
#include "UTextPoolActor.generated.h"

class UPooledUText;

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AUTextPoolActor : public AUObjectPooledSystemBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	TWeakObjectPtr<UPooledUText> GetPooledObject();

	void ReturnToPool(TWeakObjectPtr<UPooledUText> pooledText);

	TWeakObjectPtr<UPooledUText> CreateNewPooledObject();

private:
	TSubclassOf<UPooledUText> _pooledUTextClass;
};
