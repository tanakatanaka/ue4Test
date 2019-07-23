// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/ExamPlayerController.h"
#include "../Public/ExamGameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"

AExamPlayerController::AExamPlayerController()
{

}

void AExamPlayerController::BeginPlay()
{
	// Load inventory off save game before starting play
	LoadInventory();

	Super::BeginPlay();
}

bool AExamPlayerController::LoadInventory()
{
	// Fill in slots from game instance
	UWorld* World = GetWorld();
	UExamGameInstance* GameInstance = World->GetGameInstance<UExamGameInstance>();

	if (!GameInstance)
	{
		return false;
	}


	return true;
}