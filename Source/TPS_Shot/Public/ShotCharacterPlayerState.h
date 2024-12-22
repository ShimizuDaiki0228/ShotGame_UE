// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Dispose/Public/Observer.h"
#include "../Dispose/Public/Subject.h"
#include "GameFramework/PlayerState.h"
#include "ShotCharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API AShotCharacterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShotCharacterPlayerState();

	void Initialized();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

private:
	// scoreは親クラスが持っている

	TSharedPtr<Subject<int>> _scoreUpdateSubject = MakeShared<Subject<int>>();

public:
	TSharedPtr<Observer<int>> SharedScoreUpdateAsObserver = MakeShared<Observer<int>>(_scoreUpdateSubject);
	TWeakPtr<Observer<int>> TWeakScoreUpdateAsObserver = SharedScoreUpdateAsObserver;

public:
	void ChangeScore(int score);
};
