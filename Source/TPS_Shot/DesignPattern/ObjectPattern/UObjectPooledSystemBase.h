// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "UObject/Widget/PooledUserWidgetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObjectPooledSystemBase.generated.h"

inline FCriticalSection PoolLock;
class UPooledUserWidgetManager;

UCLASS()
class TPS_SHOT_API AUObjectPooledSystemBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<TWeakObjectPtr<UObject>> _pooledObjectStack;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	UObject* _pooledObject;

	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<TWeakObjectPtr<UUserWidget>> _pooledWidgetStack;

	UPROPERTY()
	TWeakObjectPtr<UPooledUserWidgetManager> _widgetManager;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<UPooledUserWidgetManager> _pooledWidgetManagerClass;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<UUserWidget> _pooledWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	int _initPoolSize = 3;

protected:
	virtual void Initialize() {}
	
public:
	template<typename T>
	TWeakObjectPtr<T> GetPooledObjectBase()
	{
		TWeakObjectPtr<T> pooledObject;
		
		if (_pooledObjectStack.Num() > 0)
		{
			pooledObject = Cast<T>(_pooledObjectStack.Pop());
		}
		// なかった場合は派生クラス先でCreateする
		
		return 	pooledObject;
	}

	template<typename T>
	TWeakObjectPtr<T> GetPooledWidgetBase()
	{
		TWeakObjectPtr<T> pooledObject;

		if (_pooledWidgetStack.Num() > 0)
		{
			UUserWidget* poppedObject = _pooledWidgetStack.Pop().Get();
			
			T* castedObject = Cast<T>(poppedObject);
			if (::IsValid(castedObject))
			{
				pooledObject = TWeakObjectPtr<T>(castedObject);
				// UKismetSystemLibrary::PrintString(this, TEXT("object is popped!"), true, true, FColor::Yellow);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, TEXT("poppedObject can't cast"), true, true, FColor::Red);
			}
		}

		return pooledObject;
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

	template<typename T>
	void ReturnToPoolWidgetBase(TWeakObjectPtr<T> object)
	{
		if (object.IsValid())
		{
			PoolLock.Lock();
			{
				FScopeLock Lock(&PoolLock);
				_pooledWidgetStack.Push(object);
				// UKismetSystemLibrary::PrintString(this, TEXT("Object is pooled"), true, true, FColor::Black);
			}
			PoolLock.Unlock();
		}
	}
};
