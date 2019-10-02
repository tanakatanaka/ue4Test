// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExamCharacter.h"
#include "Public/ExamWeapon.h"
#include "Public/CarryObjectComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Public/WeaponPickup.h"

//////////////////////////////////////////////////////////////////////////
// AExamCharacter

AExamCharacter::AExamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SocketOffset = FVector(0, 35, 0);
	CameraBoom->TargetOffset = FVector(0, 0, 55);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(GetRootComponent());	// Create a follow camera

	//CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	//CameraComp->SetupAttachment(CameraBoomComp);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AttributeSet = CreateDefaultSubobject<UExamAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->Initialize(this);

	CarriedObjectComp = CreateDefaultSubobject<UCarryObjectComponent>(TEXT("CarriedObjectComp"));
	CarriedObjectComp->SetupAttachment(GetRootComponent());

	bWantsToRun = false;
	bIsJumping = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AExamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AExamCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AExamCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AExamCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AExamCharacter::LookUpAtRate);

	// Weapons
	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &AExamCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &AExamCharacter::OnEndTargeting);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AExamCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AExamCharacter::OnStopFire);


	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AExamCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AExamCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AExamCharacter::OnResetVR);
}

bool AExamCharacter::IsTargeting() const
{
	return bIsTargeting;
}

bool AExamCharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

bool AExamCharacter::IsFiring() const
{
	return AttributeSet->CurrentWeapon && AttributeSet->CurrentWeapon->GetCurrentState() == EWeaponState::Firing;
}

bool AExamCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return bWantsToRun && !IsTargeting() && !GetVelocity().IsZero()
		// Don't allow sprint while strafing sideways or standing still (1.0 is straight forward, -1.0 is backward while near 0 is sideways or standing still)
		&& (FVector::DotProduct(GetVelocity().GetSafeNormal2D(), GetActorRotation().Vector()) > 0.8); // Changing this value to 0.1 allows for diagonal sprinting. (holding W+A or W+D keys)
}

FRotator AExamCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}


void AExamCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AExamCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AExamCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AExamCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AExamCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AExamCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AExamCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AExamCharacter::DropWeapon()
{
	if (Role < ROLE_Authority)
	{
		//ServerDropWeapon();
		return;
	}

	if (AttributeSet->CurrentWeapon)
	{
		FVector CamLoc;
		FRotator CamRot;

		if (Controller == nullptr)
		{
			return;
		}

		/* Find a location to drop the item, slightly in front of the player.
			Perform ray trace to check for blocking objects or walls and to make sure we don't drop any item through the level mesh */
		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		FVector SpawnLocation;
		FRotator SpawnRotation = CamRot;

		const FVector Direction = CamRot.Vector();
		const FVector TraceStart = GetActorLocation();
		const FVector TraceEnd = GetActorLocation() + (Direction * DropWeaponMaxDistance);

		/* Setup the trace params, we are only interested in finding a valid drop position */
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = false;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(this);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldDynamic, TraceParams);

		/* Find farthest valid spawn location */
		if (Hit.bBlockingHit)
		{
			/* Slightly move away from impacted object */
			SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * 20);
		}
		else
		{
			SpawnLocation = TraceEnd;
		}

		/* Spawn the "dropped" weapon */
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeaponPickup* NewWeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(AttributeSet->CurrentWeapon->WeaponPickupClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);

		if (NewWeaponPickup)
		{
			/* Apply torque to make it spin when dropped. */
			UStaticMeshComponent* MeshComp = NewWeaponPickup->GetMeshComponent();
			if (MeshComp)
			{
				MeshComp->SetSimulatePhysics(true);
				MeshComp->AddTorqueInRadians(FVector(1, 1, 1) * 4000000);
			}
		}

		AttributeSet->RemoveWeapon(true);
	}
}

void AExamCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	AttributeSet->SetCurrentWeapon(AttributeSet->CurrentWeapon);
}

void AExamCharacter::AddWeapon(class AExamWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		Weapon->OnEnterInventory(this);
		AttributeSet->Inventory.AddUnique(Weapon);

		// Equip first weapon in inventory
		if (AttributeSet->Inventory.Num() > 0 && AttributeSet->CurrentWeapon == nullptr)
		{
			EquipWeapon(AttributeSet->Inventory[0]);
		}
	}

}

void AExamCharacter::EquipWeapon(AExamWeapon* Weapon)
{
	if (Weapon)
	{
		/* Ignore if trying to equip already equipped weapon */
		if (Weapon == AttributeSet->CurrentWeapon) return;

		if (Role == ROLE_Authority)
		{
			AttributeSet->SetCurrentWeapon(Weapon, AttributeSet->CurrentWeapon);
		}
		else
		{
			//ServerEquipWeapon(Weapon);
		}
	}
}

float AExamCharacter::GetHealth() const
{
	return AttributeSet->Health;
}

float AExamCharacter::GetMaxHealth() const
{
	return AttributeSet->MaxHealth;
}

float AExamCharacter::GetMoveSpeed() const
{
	return AttributeSet->MoveSpeed;
}

void AExamCharacter::OnStartTargeting()
{
	if (CarriedObjectComp->GetIsCarryingActor())
	{
		CarriedObjectComp->Drop();
	}

	SetTargeting(true);
}


void AExamCharacter::OnEndTargeting()
{
	SetTargeting(false);
}



void AExamCharacter::OnStartFire()
{
	/*
	if (IsSprinting())
	{
		SetSprinting(false);
	}
	*/

	if (CarriedObjectComp->GetIsCarryingActor())
	{
		StopWeaponFire();

		CarriedObjectComp->Throw();
		return;
	}
	
	StartFire();
}

void AExamCharacter::OnStopFire()
{
	StopFire();
}

void AExamCharacter::StartFire()
{
	if (false == AttributeSet->bWantsToFire)
	{
		AttributeSet->bWantsToFire = true;
		if (AttributeSet->CurrentWeapon)
		{
			AttributeSet->CurrentWeapon->StartFire();
		}
	}
}

void AExamCharacter::StopFire()
{
	if (AttributeSet->bWantsToFire)
	{
		AttributeSet->bWantsToFire = false;
		if (AttributeSet->CurrentWeapon)
		{
			AttributeSet->CurrentWeapon->StopFire();
		}
	}
}

void AExamCharacter::SetTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;

	if (Role < ROLE_Authority)
	{
		//ServerSetTargeting(NewTargeting);
	}
}

bool AExamCharacter::CanReload() const
{
	return IsAlive();
}

bool AExamCharacter::IsAlive() const
{
	return AttributeSet->Health > 0;
}


void AExamCharacter::StopWeaponFire()
{
	if (AttributeSet->bWantsToFire)
	{
		AttributeSet->bWantsToFire = false;
		if (AttributeSet->CurrentWeapon)
		{
			AttributeSet->CurrentWeapon->StopFire();
		}
	}
}

UStaticMeshComponent* UCarryObjectComponent::GetCarriedMeshComp()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		AActor* OwningActor = ChildComp->GetOwner();
		if (OwningActor)
		{
			return Cast<UStaticMeshComponent>(OwningActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		}
	}

	return nullptr;
}

bool AExamCharacter::WeaponSlotAvailable(EInventorySlot CheckSlot)
{
	/* Iterate all weapons to see if requested slot is occupied */
	for (int32 i = 0; i < AttributeSet->Inventory.Num(); i++)
	{
		AExamWeapon* Weapon = AttributeSet->Inventory[i];
		if (Weapon)
		{
			if (Weapon->GetStorageSlot() == CheckSlot)
				return false;
		}
	}

	return true;
}



void AExamCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	/*
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASWeapon, MyPawn);

		DOREPLIFETIME_CONDITION(ASWeapon, CurrentAmmo, COND_OwnerOnly);
		DOREPLIFETIME_CONDITION(ASWeapon, CurrentAmmoInClip, COND_OwnerOnly);
		DOREPLIFETIME_CONDITION(ASWeapon, BurstCounter, COND_SkipOwner);
		DOREPLIFETIME_CONDITION(ASWeapon, bPendingReload, COND_SkipOwner);
	*/
}

FName AExamCharacter::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	return AttributeSet->GetInventoryAttachPoint(Slot);
}