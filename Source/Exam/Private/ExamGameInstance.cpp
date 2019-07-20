// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamGameInstance.h"
#include "../Public/ExamSaveGame.h"
#include "Kismet/GameplayStatics.h"

UExamGameInstance::UExamGameInstance()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{

}

void UExamGameInstance::AddDefaultInventory(UExamSaveGame* SaveGame, bool bRemoveExtra)
{
	if (bRemoveExtra)
	{
		//SaveGame->InventoryData.Reset();
	}
}

UExamSaveGame* UExamGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void UExamGameInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool UExamGameInstance::LoadOrCreateSaveGame()
{
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex)
		&& bSavingEnabled)
	{
		CurrentSaveGame = Cast<UExamSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		AddDefaultInventory(CurrentSaveGame, false);

		return true;
	}
	else
	{
		CurrentSaveGame = Cast<UExamSaveGame>(UGameplayStatics::CreateSaveGameObject(UExamSaveGame::StaticClass()));
		AddDefaultInventory(CurrentSaveGame, true);

		return false;
	}
}

bool UExamGameInstance::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex);
	}
	return false;
}

void UExamGameInstance::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}


