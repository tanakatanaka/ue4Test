// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "ZombieAIController.h"

// Sets default values
AZombieAIController::AZombieAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	BotTypeKeyName = "BotType";
	TargetEnemyKeyName = "TargetEnemy";

	/* Initializes PlayerState so we can assign a team index to AI */
	bWantsPlayerState = true;
}

void AZombieAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AZombieCharacter* ZombieBot = Cast<AZombieCharacter>(InPawn);
	if (ZombieBot)
	{
		//if (ZombieBot->BehaviorTree->BlackboardAsset)
		{
			//BlackboardComp->InitializeBlackboard(*ZombieBot->BehaviorTree->BlackboardAsset);

			/* Make sure the Blackboard has the type of bot we possessed */
			SetBlackboardBotType(ZombieBot->BotType);
		}

		BehaviorComp->StartTree(*ZombieBot->BehaviorTree);
	}
}

void AZombieAIController::OnUnPossess()
{
	Super::OnUnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}


void AZombieAIController::SetTargetEnemy(APawn* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

ABaseCharacter* AZombieAIController::GetTargetEnemy()
{
	if (BlackboardComp)
	{
		return Cast<ABaseCharacter>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}

	return nullptr;
}

void AZombieAIController::SetBlackboardBotType(EBotBehaviorType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(BotTypeKeyName, (uint8)NewType);
	}
}