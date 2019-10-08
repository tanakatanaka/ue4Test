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

	virtual float ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const;
	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType);

protected:
	virtual void RestartPlayer(class AController* NewPlayer) override;
	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;
	virtual void SpawnDefaultInventory(APawn* PlayerPawn);
	
private:
	/* Spawn at team player if any are alive */
	UPROPERTY(EditDefaultsOnly, Category = "Rules")
	bool bSpawnAtTeamPlayer;

};



