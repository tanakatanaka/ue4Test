// Fill out your copyright notice in the Description page of Project Settings.

#include "../ExamCharacter.h"
#include "ExamWeapon.h"
#include "ExamPlayerController.h"
#include "WeaponPickup.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
	bAllowRespawn = false;

	/* Enabled to support simulated physics movement when weapons are dropped by a player */
	bReplicateMovement = true;

}

void AWeaponPickup::OnUsed(APawn* InstigatorPawn)
{
	AExamCharacter* MyPawn = Cast<AExamCharacter>(InstigatorPawn);
	if (MyPawn)
	{
		/* Fetch the default variables of the class we are about to pick up and check if the storage slot is available on the pawn. */
		if (MyPawn->WeaponSlotAvailable(WeaponClass->GetDefaultObject<AExamWeapon>()->GetStorageSlot()))
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AExamWeapon* NewWeapon = GetWorld()->SpawnActor<AExamWeapon>(WeaponClass, SpawnInfo);

			MyPawn->AddWeapon(NewWeapon);

			Super::OnUsed(InstigatorPawn);
		}
		else
		{
			AExamPlayerController* PC = Cast<AExamPlayerController>(MyPawn->GetController());
			if (PC)
			{
				//PC->ClientHUDMessage(EHUDMessage::Weapon_SlotTaken);
			}
		}
	}
}

