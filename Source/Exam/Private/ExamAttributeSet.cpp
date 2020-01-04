// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/ExamAttributeSet.h"
#include "GameFramework/Controller.h"
#include "../Public/ExamPlayerController.h"
#include "../Public/ExamWeapon.h"

UExamAttributeSet::UExamAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	AttackPower = 1.f;
	DefensePower = 1.f;
	MoveSpeed = 1.f;
	Damage = 1.f;
	Hunger = 0.f;
	MaxHunger = 100.f;

	bIsEquipped = false;
	bWantsToFire = false;
}

void UExamAttributeSet::AdjustAttributeForMaxChange()
{

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