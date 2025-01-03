// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Utility/Public/TimeManagerUtility.h"
#include "../Utility/Public/ConstUtility.h"
#include "DrawDebugHelpers.h"
#include "TPS_ShotGameMode.h"
#include "GameFramework/GameMode.h"
#include "PlayerBehaviourController.h"
#include "../Utility/Public/MontageUtility.h"
#include "Utility/GameFunctionInstance.h"

//////////////////////////////////////////////////////////////////////////
// ATPS_ShotCharacter

ATPS_ShotCharacter::ATPS_ShotCharacter()
{
	_behaviourController = CreateDefaultSubobject<UPlayerBehaviourController>(TEXT("BehaviourController"));
	_bulletController = CreateDefaultSubobject<UBulletControllerComponent>(TEXT("BulletControllerComponent"));
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ATPS_ShotCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() != nullptr)
	{
		TimeManagerUtility::GetInstance().Cancel(GetWorld(), _reloadTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void ATPS_ShotCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATPS_ShotCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (auto gameModeBase = GetWorld()->GetAuthGameMode())
	{
		_shotGameMode = Cast<ATPS_ShotGameMode>(gameModeBase);
		if (!_shotGameMode)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("don't Get GameMode"), true, true, FColor::Red);
		}
	}

	_playerController = UGameFunctionInstance::GetInstance()->GetPlayerController(this);
	_animInstance = GetMesh()->GetAnimInstance();

	Reset();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPS_ShotCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Look);

		EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::CreateBullet);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Reload);
	}

}

void ATPS_ShotCharacter::CreateBullet()
{
	if (!bCanShot) return;
	if (bIsReloading) return;

	if (_bulletController->Create())
	{
		_numberOfBulletProp->SetValue(_numberOfBulletProp->GetValue() - 1);
		ChangeNotUseShot();
	}


	if (_animInstance && _hipFireMontage)
	{
		_animInstance->Montage_Play(_hipFireMontage);
		_animInstance->Montage_JumpToSection(FName("StartFire"));

		if (_numberOfBulletProp->GetValue() == 0)
		{
			MontageUtility::SetMontageEndDelegateWithLambda<TFunction<void(UAnimMontage*, bool)>>
			(
				_animInstance,
				_hipFireMontage,
				[this](UAnimMontage*, bool)
				{
					Reload();
					UE_LOG(LogTemp, Log, TEXT("Reload Animation Completed Successfully"));
				}
			);
		}
	}
}

void ATPS_ShotCharacter::ChangeNotUseShot()
{
	bCanShot = false;

	TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]()
		{
			bCanShot = _numberOfBulletProp->GetValue() > 0;
		}, 0.08f, _reloadTimerHandle);
}

void ATPS_ShotCharacter::Move(const FInputActionValue& Value)
{
	_behaviourController->Move(Value);
}

void ATPS_ShotCharacter::Look(const FInputActionValue& Value)
{
	_behaviourController->Look(Value);
}

void ATPS_ShotCharacter::Reload()
{
	if (bIsReloading) return;
	
	bIsReloading = true;
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && _reloadMontage)
	{
		UKismetSystemLibrary::PrintString(this, "Reload Animation", true, true, FColor::Cyan, 2.f, TEXT("None"));

		// TODO�@����𕡐��g�p����ꍇ�A����̃^�C�v�ɂ���ĕύX�ł���悤�ɂ���K�v������
		FName montageSection(TEXT("Reload SMG"));

		animInstance->Montage_Play(_reloadMontage);
		animInstance->Montage_JumpToSection(montageSection);
		
		MontageUtility::SetMontageEndDelegateWithLambda<TFunction<void(UAnimMontage*, bool)>>(
			animInstance,
			_reloadMontage,
			[this](UAnimMontage*, bool)
			{
				_numberOfBulletProp->SetValue(MAX_BULLET_NUMBER);
				bCanShot = true;
				bIsReloading = false;
			}
		);
	}
}

void ATPS_ShotCharacter::GrabClip()
{
	
}

void ATPS_ShotCharacter::ReleaseClip()
{
}

void ATPS_ShotCharacter::ChangeHP(int newHP)
{
}

void ATPS_ShotCharacter::Initialized()
{
	_behaviourController->Initialized(this);
	_bulletController->Initialized(this);
	Bind();
	SetEvent();
}

void ATPS_ShotCharacter::Bind()
{
	if (_numberOfBulletProp.IsValid())
	{
		_numberOfBulletProp->OnValueChanged.AddLambda([this](const int& newValue)
			{
				UKismetSystemLibrary::PrintString(this, TEXT("_numberProp is valid"), true, true, FColor::Cyan);

				_playerController->GetPlayingWidget()->ChangeBulletNumber(newValue);
			});
	}
}

void ATPS_ShotCharacter::SetEvent()
{

}

void ATPS_ShotCharacter::Reset()
{
	bCanShot = true;

	_currentLoadNumber = MAX_BULLET_NUMBER;
	_numberOfBulletProp->SetValue(MAX_BULLET_NUMBER);
	// attributeSetのHealthの値を代わりに入れる
}

void ATPS_ShotCharacter::GameOver()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMovementComponent()->MovementState.bCanJump = false;
}

void ATPS_ShotCharacter::PlayHipFireMontage()
{
}

void ATPS_ShotCharacter::HandleHealthChanged(float deltaValue, const struct FGameplayTagContainer& eventTags)
{
	Super::HandleHealthChanged(deltaValue, eventTags);

	float currentHealth = FMath::Clamp(GetHealth() + deltaValue , 0 , GetMaxHealth());

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("currentHealth : %f"), currentHealth), true, true, FColor::Cyan);

	
	if (_playerController != nullptr)
	{
		_playerController->GetPlayingWidget()->SetHP(currentHealth / GetMaxHealth());
	}
}


