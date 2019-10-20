// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/ExamAttributeSet.h"
#include "GameFramework/Controller.h"
#include "../Public/ExamPlayerController.h"
#include "../Public/ExamWeapon.h"

UExamAttributeSet::UExamAttributeSet()
{
	
}

void UExamAttributeSet::Initialize(ABaseCharacter* _owner)
{
	Health = 1.f;
	MaxHealth = 1.f;
	AttackPower = 1.f;
	DefensePower = 1.f;
	MoveSpeed = 1.f;
	Damage = 1.f;
	Hunger = 100.f;
	MaxHunger = 100.f;

	bIsEquipped = false;
	bWantsToFire = false;

	/* Names as specified in the character skeleton */
	WeaponAttachPoint = TEXT("WeaponSocket");
	PelvisAttachPoint = TEXT("PelvisSocket");
	SpineAttachPoint = TEXT("SpineSocket");

	Owner = _owner;
}

void UExamAttributeSet::AdjustAttributeForMaxChange()
{

}


void UExamAttributeSet::SetCurrentWeapon(class AExamWeapon* NewWeapon, class AExamWeapon* LastWeapon)
{
	/* Maintain a reference for visual weapon swapping */
	PreviousWeapon = LastWeapon;

	AExamWeapon* LocalLastWeapon = nullptr;
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// UnEquip the current
	bool bHasPreviousWeapon = false;
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
		bHasPreviousWeapon = true;
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn((AExamCharacter*)Owner);
		NewWeapon->OnEquip(bHasPreviousWeapon);
	}

	/* NOTE: If you don't have an equip animation w/ animnotify to swap the meshes halfway through, then uncomment this to immediately swap instead */
	//SwapToNewWeaponMesh();
}

void UExamAttributeSet::RemoveWeapon(bool bDestroy)
{
	if (CurrentWeapon)
	{
		bool bIsCurrent = true;

		if (Inventory.Contains(CurrentWeapon))
		{
			CurrentWeapon->OnLeaveInventory();
		}
		Inventory.RemoveSingle(CurrentWeapon);

		/* Replace weapon if we removed our current weapon */
		if (bIsCurrent && Inventory.Num() > 0)
		{
			SetCurrentWeapon(Inventory[0]);
		}

		/* Clear reference to weapon if we have no items left in inventory */
		if (Inventory.Num() == 0)
		{
			SetCurrentWeapon(nullptr);
			bDestroy = false;
		}

		if (bDestroy)
		{
			CurrentWeapon->Destroy();
		}
	}

}


FName UExamAttributeSet::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	/* Return the socket name for the specified storage slot */
	switch (Slot)
	{
	case EInventorySlot::Hands:
		return WeaponAttachPoint;
	case EInventorySlot::Primary:
		return SpineAttachPoint;
	case EInventorySlot::Secondary:
		return PelvisAttachPoint;
	default:
		// Not implemented.
		return "";
	}
}

void UExamAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UExamAttributeSet::RestoreCondition(float HealthRestored, float HungerRestored)
{
	// Reduce Hunger, ensure we do not go outside of our bounds
	Hunger = FMath::Clamp(Hunger - HungerRestored, 0.0f, MaxHunger);

	// Restore Hitpoints
	Health = FMath::Clamp(Health + HealthRestored, 0.0f, MaxHealth);
}