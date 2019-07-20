// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamSaveGame.h"

void UExamSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != ExamSaveGameVersion::LatestVersion)
	{
		for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
		{
			//InventoryData.Add(ItemId, FRPGItemData(1, 1));
		}

		InventoryItems_DEPRECATED.Empty();

	}

}