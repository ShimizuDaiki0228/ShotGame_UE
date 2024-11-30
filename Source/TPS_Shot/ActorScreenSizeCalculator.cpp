// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorScreenSizeCalculator.h"

#include "GameFramework/PlayerController.h"

// 画面に描画されているアクターのサイズを取得
float UActorScreenSizeCalculator::CalculateScreenSize(AActor* actor, APlayerController* playerController, float clampMin, float clampMax)
{
	FVector2D min, max;
	if (GetScreenBounds(actor, playerController, min, max))
	{
		float thisScreenSize = FMath::Abs(max.X - min.X);
		thisScreenSize = FMath::Clamp(thisScreenSize, clampMin, clampMax);
		return thisScreenSize;
	}
	
	return 0;
}

bool UActorScreenSizeCalculator::GetScreenBounds(AActor* actor, APlayerController* playerController,
	FVector2D& outMin, FVector2D& outMax)
{
	if (playerController && actor)
	{
		FVector origin, boxExtent;
		actor->GetActorBounds(false, origin, boxExtent);

		// カメラの距離でアクターの描画サイズが変わるのでそれに合わせる
		FVector cameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
		float distanceToCamera = FVector::Dist(cameraLocation, origin);
		
		float scaledSize = BASE_SIZE * (SCALE_FACTOR / distanceToCamera);
		
		outMin = FVector2D(-scaledSize / 2, -scaledSize / 2);
		outMax = FVector2D(scaledSize / 2, scaledSize / 2); 
		
		return true;
	}

	return false;
}
