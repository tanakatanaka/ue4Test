// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ExamGameInstance.generated.h"

class UExamSaveGame;


/**
 * 
 */
UCLASS()
class EXAM_API UExamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UExamGameInstance();

	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	/**
	 * Adds the default inventory to the inventory array
	 * @param InventoryArray Inventory to modify
	 * @param RemoveExtra If true, remove anything other than default inventory
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultInventory(UExamSaveGame* SaveGame, bool bRemoveExtra = false);

	/** Returns the current save game, so it can be used to initialize state. Changes are not written until WriteSaveGame is called */
	UFUNCTION(BlueprintCallable, Category = Save)
	UExamSaveGame* GetCurrentSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void SetSavingEnabled(bool bEnabled);

	/** Loads a save game. If it fails, it will create a new one for you. Returns true if it loaded, false if it created one */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	/** Writes the current save game object to disk */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame();

protected:
	/** The current save game object */
	UPROPERTY()
	UExamSaveGame* CurrentSaveGame;

	/** Rather it will attempt to actually save to disk */
	UPROPERTY()
	bool bSavingEnabled;
};
