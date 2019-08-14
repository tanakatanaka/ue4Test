// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamAttributeSet.h"
#include "../Public/ExamWeapon.h"

UExamAttributeSet::UExamAttributeSet()
{
	Health = 1.f;
	MaxHealth = 1.f;
	AttackPower = 1.f;
	DefensePower = 1.f;
	MoveSpeed = 1.f;
	Damage = 1.f;

	bIsEquipped = false;
	bWantsToFire = false;

	/* Names as specified in the character skeleton */
	WeaponAttachPoint = TEXT("WeaponSocket");
	PelvisAttachPoint = TEXT("PelvisSocket");
	SpineAttachPoint = TEXT("SpineSocket");


}

bool UExamAttributeSet::IsAlive()
{
	return Health > 0;
}


void UExamAttributeSet::AdjustAttributeForMaxChange()
{

}


void UExamAttributeSet::SetCurrentWeapon(AExamCharacter* Owner, class AExamWeapon* NewWeapon, class AExamWeapon* LastWeapon)
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
		NewWeapon->SetOwningPawn(Owner);
		NewWeapon->OnEquip(bHasPreviousWeapon);
	}

	/* NOTE: If you don't have an equip animation w/ animnotify to swap the meshes halfway through, then uncomment this to immediately swap instead */
	//SwapToNewWeaponMesh();
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