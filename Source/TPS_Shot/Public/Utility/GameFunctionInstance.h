// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerControllerBase.h"
#include "TPS_ShotGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "GameFunctionInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_SHOT_API UGameFunctionInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	
	
	static UGameFunctionInstance* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = NewObject<UGameFunctionInstance>();
			_instance->AddToRoot();
		}
		return _instance;
	}

	static UGameFunctionInstance* _instance;

private:
	void Initialized();
	
public:
	FORCEINLINE ATPS_ShotGameMode* GetMyGameMode(AActor* owner) const
	{
		if (owner->GetWorld())
		{
			auto* gameMode = UGameplayStatics::GetGameMode(owner->GetWorld());
			auto* myGameMode = Cast<ATPS_ShotGameMode>(gameMode);
			if (myGameMode)
			{
				return myGameMode;
			}
			UE_LOG(LogTemp, Error, TEXT("myGameMode don't get"));
			UKismetSystemLibrary::PrintString(this, TEXT("myGameMode don't get"), true, true, FColor::Red);

			return nullptr;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[GetMyGameMode]GetWorld is null"));
			return nullptr;
		}
	}
	
	FORCEINLINE AMyPlayerControllerBase* GetPlayerController(AActor* owner) const
	{
		if (owner->GetWorld())
		{

			auto myPlayerController = GetMyGameMode(owner)->GetPlayerController();
			if (::IsValid(myPlayerController))
			{
				return myPlayerController;
			}
			
			auto playerController = UGameplayStatics::GetPlayerController(owner->GetWorld(), 0);
			if (playerController)
			{
				myPlayerController = Cast<AMyPlayerControllerBase>(playerController);
				if (myPlayerController)
				{
					return myPlayerController;
				}
			}

			UE_LOG(LogTemp, Error, TEXT("myPlayerController don't get"));
			return nullptr;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[GetPlayerController]GetWorld is null"));
			return nullptr;
		}
	}
};
