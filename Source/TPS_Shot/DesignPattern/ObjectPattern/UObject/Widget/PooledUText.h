// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../UPooledObjectBase.h"
#include "UTextPoolActor.h"
#include "PooledUText.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UPooledUText : public UUserWidget, public UPooledObjectBase<AUTextPoolActor>
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AUTextPoolActor* Pool;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _pooledText;
	

public:
	UTextBlock* GetTextBlock() const { return _pooledText; }

	void Release();
};
