// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPooledParticleSystemComponent::UPooledParticleSystemComponent()
{
	OnSystemFinished.AddDynamic(this, &UPooledParticleSystemComponent::Release);
}

void UPooledParticleSystemComponent::Release(UParticleSystemComponent* PSystem)
{
	// UKismetSystemLibrary::PrintString(this, TEXT("particle system finished"), true, true, FColor::Red, 2.f);

	Pool->ReturnToPool(this);
}
