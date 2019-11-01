// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API AEPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	void AddKill();

	void AddDeath();

	void ScorePoints(int32 Points);

	void SetTeamNumber(int32 NewTeamNumber);

	UFUNCTION(BlueprintCallable, Category = "Teams")
	int32 GetTeamNumber() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetKills() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetDeaths() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	float GetScore() const;
	
private:
	AEPlayerState();

	UPROPERTY(Transient, Replicated)
	int32 NumKills;

	UPROPERTY(Transient, Replicated)
	int32 NumDeaths;

	/* Team number assigned to player */
	UPROPERTY(Transient, Replicated)
	int32 TeamNumber;

	virtual void Reset() override;
	
};
