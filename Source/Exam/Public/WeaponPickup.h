// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.h"
#include "WeaponPickup.generated.h"

class AExamWeapon;

UCLASS()
class EXAM_API AWeaponPickup : public APickupActor
{
	GENERATED_BODY()

public:	
	/* Class to add to inventory when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<AExamWeapon> WeaponClass;

	virtual void OnUsed(APawn* InstigatorPawn) override;
private:
	AWeaponPickup();

};
