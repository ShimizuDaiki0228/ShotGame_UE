#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PooledObjectActor.h"
#include "ObjectPoolActor.generated.h"

class APooledObjectActor;

UCLASS()
class TPS_SHOT_API AObjectPoolActor : public AActor
{
	GENERATED_BODY()

protected:
	// To add mapping context
	virtual void BeginPlay();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int _initPoolSize = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	TSubclassOf<APooledObjectActor> _objectToPool;

	TArray<APooledObjectActor*> _pooledObjectStack;

	APooledObjectActor* CreateNewPooledObject();

	void ActiveChange(APooledObjectActor* pooledObjectActor, bool isPopObject);

public:
	void ReturnToPool(APooledObjectActor* pooledObject);

	APooledObjectActor* GetPooledObject();
};
