// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ExamGameMode.generated.h"

class AExamWeapon;


UCLASS(minimalapi)
class AExamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AExamGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<class AExamWeapon>> DefaultInventoryClasses;

protected:
	virtual void RestartPlayer(class AController* NewPlayer) override;
	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;
	virtual void SpawnDefaultInventory(APawn* PlayerPawn);

private:
	/* Spawn at team player if any are alive */
	UPROPERTY(EditDefaultsOnly, Category = "Rules")
	bool bSpawnAtTeamPlayer;

};



