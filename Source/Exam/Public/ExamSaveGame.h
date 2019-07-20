// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ExamSaveGame.generated.h"

/**
 * 
 */
namespace ExamSaveGameVersion
{
	enum type
	{
		// Initial version
		Initial,
		// -----<new versions must be added before this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
}

UCLASS(BlueprintType)
class EXAM_API UExamSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UExamSaveGame()
	{
		// Set to current version, this will get overwritten during serialization when loading
		SavedDataVersion = ExamSaveGameVersion::LatestVersion;
	}

protected:
	/** Deprecated way of storing items, this is read in but not saved out */
	UPROPERTY()
	TArray<FPrimaryAssetId> InventoryItems_DEPRECATED;

	/** What LatestVersion was when the archive was saved */
	UPROPERTY()
	int32 SavedDataVersion;

	/** Overridden to allow version fixups */
	virtual void Serialize(FArchive& Ar) override;
};
