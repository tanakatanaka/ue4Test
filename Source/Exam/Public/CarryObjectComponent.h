// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "CarryObjectComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API UCarryObjectComponent : public USpringArmComponent
{
	GENERATED_BODY()

	UCarryObjectComponent(const FObjectInitializer& ObjectInitializer);
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float MaxPickupDistance;

	bool GetIsCarryingActor();
	void Pickup();
	void Drop();
	void Throw();

	UFUNCTION(Reliable, NetMulticast)
	void OnDropMulticast();
	void OnDropMulticast_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotateSpeed;


private:

	AActor* GetActorInView();
	AActor* GetCarriedActor();

	/* Return the StaticMeshComponent of the carried Actor */
	UStaticMeshComponent* GetCarriedMeshComp();


};
