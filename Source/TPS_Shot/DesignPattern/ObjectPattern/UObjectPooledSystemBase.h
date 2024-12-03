// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObjectPooledSystemBase.generated.h"

inline FCriticalSection PoolLock;

template<typename TObjectPoolClass>
class UPooledObjectBase;

UCLASS()
class TPS_SHOT_API AUObjectPooledSystemBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<TWeakObjectPtr<UObject>> _pooledObjectStack;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	UObject* _pooledObject;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	int _initPoolSize = 3;

protected:
	virtual void Initialize() {}
	
public:
	template<typename T>
	TWeakObjectPtr<T> GetPooledObjectBase(const AActor* owner)
	{
		TWeakObjectPtr<T> pooledObject;
		
		if (_pooledObjectStack.Num() > 0)
		{
			pooledObject = Cast<T>(_pooledObjectStack.Pop());
		}
		// なかった場合は派生クラス先でCreateする
		
		return 	pooledObject;
	}
	
	///GetPooledObjectとタイミングがかぶった時に_pooledObjectStack.Num() == 0の部分でクラッシュしていそう
	///現状プールの中で保持しているオブジェクト数を増やすことで対処しているが今後考える必要性はある
	template<typename T>
	void ReturnToPoolBase(TWeakObjectPtr<T> object)
	{
		if (object.IsValid())
		{
			PoolLock.Lock();
			{
				FScopeLock Lock(&PoolLock);
				_pooledObjectStack.Push(object);
			}
			PoolLock.Unlock();
		}
	}
};
