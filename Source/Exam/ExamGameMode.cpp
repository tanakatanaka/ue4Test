// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExamGameMode.h"
#include "ExamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AExamGameMode::AExamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
