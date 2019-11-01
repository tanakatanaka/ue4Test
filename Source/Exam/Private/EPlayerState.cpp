// Fill out your copyright notice in the Description page of Project Settings.


#include "EPlayerState.h"

AEPlayerState::AEPlayerState()
{
	/* AI will remain in team 0, players are updated to team 1 through the GameMode::InitNewPlayer */
	TeamNumber = 0;
}


void AEPlayerState::Reset()
{
	Super::Reset();

	NumKills = 0;
	NumDeaths = 0;
	Score = 0;
}


void AEPlayerState::AddKill()
{
	NumKills++;
}

void AEPlayerState::AddDeath()
{
	NumDeaths++;
}

void AEPlayerState::ScorePoints(int32 Points)
{
	Score += Points;

	/* Add the score to the global score count */
	/*
	AGameState* GS = GetWorld()->GetGameState<ASGameState>();
	if (GS)
	{
		GS->AddScore(Points);
	}
	*/
}

void AEPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}


int32 AEPlayerState::GetTeamNumber() const
{
	return TeamNumber;
}

int32 AEPlayerState::GetKills() const
{
	return NumKills;
}

int32 AEPlayerState::GetDeaths() const
{
	return NumDeaths;
}

float AEPlayerState::GetScore() const
{
	return Score;
}

void AEPlayerState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AEPlayerState, NumKills);
	//DOREPLIFETIME(AEPlayerState, NumDeaths);
	//DOREPLIFETIME(AEPlayerState, TeamNumber);
}