// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExamPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API AExamPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AExamPlayerController();
	virtual void BeginPlay() override;

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

};
