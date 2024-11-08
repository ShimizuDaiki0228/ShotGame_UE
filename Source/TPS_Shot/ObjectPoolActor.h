#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObjectActor.h"
#include "ObjectPoolActor.generated.h"

UCLASS()
class TPS_SHOT_API AObjectPoolActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int _initPoolSize;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	APooledObjectActor* _objectToPool;

	TArray<APooledObjectActor*> _pooledObjectStack;

public:
	void ReturnToPool(APooledObjectActor* pooledObject);
};
