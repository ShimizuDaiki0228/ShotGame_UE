#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ObjectPoolActor.generated.h"

class APooledObject;

UCLASS()
class TPS_SHOT_API AObjectPoolActor : public AActor
{
	GENERATED_BODY()

protected:
	// To add mapping context
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Pool")
	int _initPoolSize = 3;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<AActor> _objectToPool;

	UPROPERTY()
	TArray<AActor*> _pooledObjectStack;

	AActor* CreateNewPooledObject();

	static void ActiveChange(AActor* pooledObjectActor, bool isPopObject);

public:
	void ReturnToPool(AActor* pooledObject);

	template<typename T>
	T* GetPooledObject(const AActor* owner)
	{
		AActor* object;
	
		if (_pooledObjectStack.Num() == 0)
		{
			object = CreateNewPooledObject();
		}
		else
		{
			object = _pooledObjectStack.Pop();
		}
	
		T* pooledObject = static_cast<T*>(object);
		if (pooledObject)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("create"), true, true, FColor::Yellow);
			ActiveChange(pooledObject, true);
			pooledObject->SetActorLocation(owner->GetActorLocation());
			return pooledObject;
		}
		
		UKismetSystemLibrary::PrintString(this, TEXT("don't get pooledObject"), true, true, FColor::Red);
		return nullptr;
	}
};
