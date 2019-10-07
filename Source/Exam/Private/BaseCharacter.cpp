// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ExamAttributeSet.h"
#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	AttributeSet = CreateDefaultSubobject<UExamAttributeSet>(TEXT("AttributeSet"));
	//AttributeSet->Initialize(this);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


float ABaseCharacter::GetHealth() const
{
	return AttributeSet->Health;
}

float ABaseCharacter::GetMaxHealth() const
{
	return AttributeSet->MaxHealth;
}

bool ABaseCharacter::IsAlive() const
{
	return AttributeSet->Health > 0;
}

bool ABaseCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return bWantsToRun && !IsTargeting() && !GetVelocity().IsZero()
		// Don't allow sprint while strafing sideways or standing still (1.0 is straight forward, -1.0 is backward while near 0 is sideways or standing still)
		&& (FVector::DotProduct(GetVelocity().GetSafeNormal2D(), GetActorRotation().Vector()) > 0.8); // Changing this value to 0.1 allows for diagonal sprinting. (holding W+A or W+D keys)
}

bool ABaseCharacter::IsTargeting() const
{
	return bIsTargeting;
}

FRotator ABaseCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}
