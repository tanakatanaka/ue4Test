// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotWaypoint.h"
#include "BaseCharacter.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ZombieAIController.generated.h"

class AZombieCharacter;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class EXAM_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABotWaypoint* GetWaypoint();

	ABaseCharacter* GetTargetEnemy();

	void SetWaypoint(ABotWaypoint* NewWaypoint);

	void SetTargetEnemy(APawn* NewTarget);

	void SetBlackboardBotType(EBotBehaviorType NewType);

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

private:
	AZombieAIController();
	
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BotTypeKeyName;

};
