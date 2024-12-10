// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UObjectPooledSystemBase.h"
#include "UTextPoolActor.generated.h"

class UPooledUText;
class UCharacterWidgetController;

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
	TWeakObjectPtr<UPooledUText> GetPooledObject(const FString& text, UCharacterWidgetController* widgetController);

	void ReturnToPool(TWeakObjectPtr<UPooledUText> pooledText);

	TWeakObjectPtr<UPooledUText> CreateNewPooledObject();

private:
	TSubclassOf<UPooledUText> _pooledUTextClass;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> _cachedPlayerController;
};
