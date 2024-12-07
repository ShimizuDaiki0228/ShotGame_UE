// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CanvasPanel.h"
#include "PooledUserWidgetManager.generated.h"

class AUTextPoolActor;
class UPooledUText;

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UPooledUserWidgetManager : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	template<typename T, typename TClass>
	TWeakObjectPtr<UPooledUText> TextInitialized(T* pooledObjectBase, TSubclassOf<TClass> pooledObjectClass)
	{
		// コンパイル時点で正しく継承していないと引っかかる
		static_assert(std::is_base_of_v<AUTextPoolActor, T>, "T must derive from AUObjectPooledSystemBase");
		static_assert(std::is_base_of_v<UPooledUText, TClass>, "TClass must derive from UPooledUText");

		return PooledTextCreate(pooledObjectBase, pooledObjectClass);
	}

	TWeakObjectPtr<UPooledUText> PooledTextCreate(
		AUTextPoolActor* pooledObjectBase,
		TSubclassOf<UPooledUText> pooledObjectClass);

private:
	UPROPERTY()
	UCanvasPanel* _canvasPanel;

	bool bIsConstructed;
	
public:
	bool GetIsConstructed() const {return bIsConstructed;}
};
