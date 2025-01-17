// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanelSlot.h"
#include "UObject/Interface.h"
#include "IFollowActorWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIFollowActorWidget : public UInterface
{
	GENERATED_BODY()
};

class TPS_SHOT_API IIFollowActorWidget
{
	GENERATED_BODY()

public:
	virtual void SetSize(const FVector2D& size) = 0;
};
