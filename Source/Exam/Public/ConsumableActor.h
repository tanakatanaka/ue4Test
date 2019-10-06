// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"
#include "ConsumableActor.generated.h"

UCLASS()
class EXAM_API AConsumableActor : public APickupActor
{
	GENERATED_BODY()
	
	AConsumableActor();

protected:
	/* Consume item, restoring energy to player */
	virtual void OnUsed(APawn* InstigatorPawn) override;

	/* Amount of hitpoints restored and hunger reduced when consumed. */
	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	float Nutrition;

};
