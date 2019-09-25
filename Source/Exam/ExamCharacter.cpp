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
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AttributeSet = CreateDefaultSubobject<UExamAttributeSet>(TEXT("AttributeSet"));
	
	CarriedObjectComp = CreateDefaultSubobject<UCarryObjectComponent>(TEXT("CarriedObjectComp"));
	CarriedObjectComp->SetupAttachment(GetRootComponent());

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

void AExamCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	SetCurrentWeapon(AttributeSet->CurrentWeapon);
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

void AExamCharacter::SetCurrentWeapon(class AExamWeapon* NewWeapon, class AExamWeapon* LastWeapon)
{
	/* Maintain a reference for visual weapon swapping */
	AttributeSet->PreviousWeapon = LastWeapon;

	AExamWeapon* LocalLastWeapon = nullptr;
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != AttributeSet->CurrentWeapon)
	{
		LocalLastWeapon = AttributeSet->CurrentWeapon;
	}

	// UnEquip the current
	bool bHasPreviousWeapon = false;
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
		bHasPreviousWeapon = true;
	}

	AttributeSet->CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		/* Only play equip animation when we already hold an item in hands */
		NewWeapon->OnEquip(bHasPreviousWeapon);
	}

	/* NOTE: If you don't have an equip animation w/ animnotify to swap the meshes halfway through, then uncomment this to immediately swap instead */
	//SwapToNewWeaponMesh();
}

void AExamCharacter::EquipWeapon(AExamWeapon* Weapon)
{
	if (Weapon)
	{
		/* Ignore if trying to equip already equipped weapon */
		if (Weapon == AttributeSet->CurrentWeapon) return;

		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(Weapon, AttributeSet->CurrentWeapon);
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