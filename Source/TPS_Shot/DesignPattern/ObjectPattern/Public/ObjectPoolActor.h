#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolActor.generated.h"

// class APooledObjectActor;
class UPooledObjectActorComponent;

UCLASS()
class TPS_SHOT_API AObjectPoolActor : public AActor
{
	GENERATED_BODY()

protected:
	// To add mapping context
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int _initPoolSize = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	TSubclassOf<AActor> _objectToPool;

	TArray<UPooledObjectActorComponent*> _pooledObjectStack;

	UPooledObjectActorComponent* CreateNewPooledObject();

	static void ActiveChange(AActor* pooledObjectActor, bool isPopObject);

public:
	void ReturnToPool(UPooledObjectActorComponent* pooledObject);

	UPooledObjectActorComponent* GetPooledObject(const AActor* owner);
};
