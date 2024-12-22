// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RayObject.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API URayObject : public UObject
{
	GENERATED_BODY()
	
public:
	URayObject() {}

	bool IsHit(const AActor* shotActor,
		const AActor* targetActor,
		const UWorld* world);

private:
	const FColor _rayColor = FColor::Red;
};
