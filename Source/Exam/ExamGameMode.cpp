// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExamGameMode.h"
#include "ExamCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ExamCharacter.h"
#include "public/ExamWeapon.h"
#include "Runtime/Engine/Classes/Engine/World.h"

AExamGameMode::AExamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AExamGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	//Super::SetPlayerDefaults(PlayerPawn);

	SpawnDefaultInventory(PlayerPawn);
}


void AExamGameMode::SpawnDefaultInventory(APawn* PlayerPawn)
{
	AExamCharacter* MyPawn = Cast<AExamCharacter>(PlayerPawn);
	if (MyPawn)
	{
		for (int32 i = 0; i < DefaultInventoryClasses.Num(); i++)
		{
			if (DefaultInventoryClasses[i])
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AExamWeapon* NewWeapon = GetWorld()->SpawnActor<AExamWeapon>(DefaultInventoryClasses[i], SpawnInfo);

				MyPawn->AddWeapon(NewWeapon);
			}
		}
	}
}