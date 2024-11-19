// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPooledParticleSystemComponent::UPooledParticleSystemComponent()
{
	OnSystemFinished.AddDynamic(this, &UPooledParticleSystemComponent::OnParticleSystemFinished);
}

void UPooledParticleSystemComponent::OnParticleSystemFinished(UParticleSystemComponent* PSystem)
{
	UKismetSystemLibrary::PrintString(this, TEXT("object return to pool"), true, true, FColor::Red, 2.f);

	Pool->ReturnToPool(this);
}
