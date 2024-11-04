// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/TimeManagerUtility.h"
#include "Utility/ConstUtility.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy/EnemyActor.h"

//////////////////////////////////////////////////////////////////////////
// ATPS_ShotCharacter

ATPS_ShotCharacter::ATPS_ShotCharacter()
{
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


	ChangeNotUseShot();

	if (_fireSound)
	{
		SoundManagerUtility::GetInstance().Play(_fireSound, this);
	}

	const USkeletalMeshSocket* barrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (barrelSocket)
	{
		const FTransform socketTransform = barrelSocket->GetSocketTransform(GetMesh());

		if (_muzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _muzzleFlash, socketTransform);
		}

		_numberOfBulletProp->SetValue(_numberOfBulletProp->GetValue() - 1);
		bIsReloading = ChangeNumberOfBullet(_numberOfBulletProp->GetValue());

		FVector beamEnd;
		bool bBeamEnd = GetBeamEndLocation(socketTransform.GetLocation(), beamEnd);
		if (bBeamEnd)
		{
			if (_impatctParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _impatctParticle, beamEnd);
			}

			if (_beamParticle)
			{
				UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					_beamParticle,
					socketTransform);
				if (beam)
				{
					beam->SetVectorParameter(FName("Target"), beamEnd);
				}
			}
		}
		
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && _hipFireMontage)
		{
			animInstance->Montage_Play(_hipFireMontage);
			animInstance->Montage_JumpToSection(FName("StartFire"));
		}
	}
}

void ATPS_ShotCharacter::ChangeNotUseShot()
{
	bCanShot = false;

	TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]()
		{
			bCanShot = true;
		}, 0.2f, _reloadTimerHandle);
}

bool ATPS_ShotCharacter::ChangeNumberOfBullet(int currentNumberOfBullet)
{
	if (currentNumberOfBullet <= 0)
	{
		// 非同期でリロードアニメーションとリロードテキストアニメーションを行ってbIsReloadingをfalseにしたい
		return true;
	}
	return false;
}



void ATPS_ShotCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPS_ShotCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPS_ShotCharacter::Reload()
{
	UKismetSystemLibrary::PrintString(this, "Reload", true, true, FColor::Cyan, 2.f, TEXT("None"));
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && _reloadMontage)
	{
		UKismetSystemLibrary::PrintString(this, "Reload Animation", true, true, FColor::Cyan, 2.f, TEXT("None"));

		// TODO　武器を複数使用する場合、武器のタイプによって変更できるようにする必要がある
		FName montageSection(TEXT("Reload SMG"));

		animInstance->Montage_Play(_reloadMontage);
		animInstance->Montage_JumpToSection(montageSection);
	}
}

void ATPS_ShotCharacter::ChangeHP(int newHP)
{
	_currentHPProp->SetValue(newHP);
}

void ATPS_ShotCharacter::Initialized()
{
	Bind();
}

void ATPS_ShotCharacter::Bind()
{
	
}

void ATPS_ShotCharacter::Reset()
{
	bCanShot = true;

	_currentLoadNumber = MAX_BULLET_NUMBER;
	_numberOfBulletProp->SetValue(MAX_BULLET_NUMBER);
	_currentHPProp->SetValue(ConstUtility::PLAYER_MAX_HP);
}

bool ATPS_ShotCharacter::GetBeamEndLocation(const FVector& muzzleSocketLocation, FVector& outBeamLocation)
{
	FVector2D viewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewPortSize);
	}

	FVector2D crosshairLocation = FVector2D(viewPortSize.X / 2, viewPortSize.Y / 2);
	FVector crosshairWorldPosition;
	FVector crosshairWorldDirection;

	// スクリーン座標をワールド座標に変換
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		crosshairLocation,
		crosshairWorldPosition,
		crosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult screenTraceHit;
		const FVector start = crosshairWorldPosition;
		const FVector end = crosshairWorldPosition + crosshairWorldDirection * 50'000.f;

		outBeamLocation = end;

		GetWorld()->LineTraceSingleByChannel(screenTraceHit, start, end, ECollisionChannel::ECC_Visibility);

		if (screenTraceHit.bBlockingHit)
		{
			outBeamLocation = screenTraceHit.Location;

			AActor* hitActor = screenTraceHit.GetActor();
			if (hitActor)
			{
				AEnemyActor* enemy = Cast<AEnemyActor>(hitActor);
				if (enemy)
				{
					if (enemy->DecreaseHP())
					{
						enemy->SelfDestroy();
						_scoreProp->SetValue(_scoreProp->GetValue()+1);
					}
				}
			}
		}

		// クロスヘアからのトレースと武器からのトレースで差異が生まれるためより正確にするために
		FHitResult weaponTraceHit;
		const FVector weaponTraceStart = muzzleSocketLocation;
		const FVector weaponTraceEnd = outBeamLocation;
		GetWorld()->LineTraceSingleByChannel(
			weaponTraceHit,
			weaponTraceStart,
			weaponTraceEnd,
			ECollisionChannel::ECC_Visibility
		);
		if (weaponTraceHit.bBlockingHit)
		{
			outBeamLocation = weaponTraceHit.Location;
		}

		return true;
	}

	return false;
}

