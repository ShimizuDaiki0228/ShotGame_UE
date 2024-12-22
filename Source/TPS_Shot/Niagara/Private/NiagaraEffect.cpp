// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/NiagaraEffect.h"

ANiagaraEffect::ANiagaraEffect() : ANiagaraActor(FObjectInitializer::Get())
{
	if (GetRootComponent() == nullptr)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	
	GetNiagaraComponent()->SetupAttachment(RootComponent);
}
