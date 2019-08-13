// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CarryObjectComponent.h"

UCarryObjectComponent::UCarryObjectComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxPickupDistance = 600;
	RotateSpeed = 10.0f;

	bUsePawnControlRotation = true;
	bDoCollisionTest = false;

	SetIsReplicated(true);
}

bool UCarryObjectComponent::GetIsCarryingActor()
{
	return GetChildComponent(0) != nullptr;
}


void UCarryObjectComponent::Drop()
{
	/*
	if (GetOwner()->Role < ROLE_Authority)
	{
		ServerDrop();
	}
	*/
	//OnDropMulticast();
}

AActor* UCarryObjectComponent::GetCarriedActor()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		return ChildComp->GetOwner();
	}

	return nullptr;
}


void UCarryObjectComponent::Throw()
{
	if (!GetIsCarryingActor())
		return;
	/*
	if (GetOwner()->Role < ROLE_Authority)
	{
		ServerThrow();
		return;
	}
	*/
	/* Grab a reference to the MeshComp before dropping the object */
	UStaticMeshComponent* MeshComp = GetCarriedMeshComp();
	if (MeshComp)
	{
		/* Detach and re-enable collision */
		OnDropMulticast();

		APawn* OwningPawn = Cast<APawn>(GetOwner());
		if (OwningPawn)
		{
			/* Re-map uint8 to 360 degrees */
			const float PawnViewPitch = (OwningPawn->RemoteViewPitch / 255.f)*360.f;

			FRotator NewRotation = GetComponentRotation();
			NewRotation.Pitch = PawnViewPitch;

			/* Apply physics impulse, ignores mass */
			MeshComp->AddImpulse(NewRotation.Vector() * 1000, NAME_None, true);
		}
	}
}

void UCarryObjectComponent::OnDropMulticast_Implementation()
{
#if 0
	AActor* CarriedActor = GetCarriedActor();
	if (CarriedActor)
	{
		/* Find the static mesh (if any) to re-enable physics simulation */
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(CarriedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComp)
		{
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComp->SetSimulatePhysics(true);
		}

		CarriedActor->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
#endif
}