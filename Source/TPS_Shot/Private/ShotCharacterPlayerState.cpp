// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotCharacterPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"

AShotCharacterPlayerState::AShotCharacterPlayerState()
{
	SetScore(0);
}

void AShotCharacterPlayerState::Initialized()
{
	// UKismetSystemLibrary::PrintString(this, TEXT("PlayerState Initialized"), true, true, FColor::Green);
}

void AShotCharacterPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AShotCharacterPlayerState::BeginDestroy()
{
	Super::BeginDestroy();

	_scoreUpdateSubject->ShutDown();
}

void AShotCharacterPlayerState::ChangeScore(int score)
{
	SetScore(score);
	_scoreUpdateSubject->Notify(score);
}

