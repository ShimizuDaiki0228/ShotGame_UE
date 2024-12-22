// Fill out your copyright notice in the Description page of Project Settings.


#include "RayObject.h"
#include "Kismet/KismetSystemLibrary.h"

bool URayObject::IsHit(const AActor* shotActor, const AActor* targetActor, const UWorld* world)
{
	if (targetActor == nullptr)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("target Actor is Null"), true, true, FColor::Red, 2.0f);
		return false;
	}


	FVector startPosition = shotActor->GetActorLocation();
	FVector endPosition = targetActor->GetActorLocation();

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(shotActor);

	bool bHit = world->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECC_Visibility, collisionParams);

	if (bHit && hitResult.GetActor() == targetActor)
	{
		if (hitResult.GetActor())
		{
			//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName()), true, true, FColor::Green, 2.0f);
		}

		DrawDebugLine(world, startPosition, endPosition, _rayColor, false, 0.1f, 0, 2.0f);
		return true;
	}

	//UKismetSystemLibrary::PrintString(this, TEXT("false"), true, true, FColor::Red, 2.0f);

	return false;
}
