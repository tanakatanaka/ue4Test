#include "ConsumableActor.h"
#include "ExamCharacter.h"


// Sets default values
AConsumableActor::AConsumableActor()
{
	Nutrition = 20;

	bAllowRespawn = true;
	RespawnDelay = 60.0f;
	RespawnDelayRange = 20.0f;

}

void AConsumableActor::OnUsed(APawn* InstigatorPawn)
{
	AExamCharacter* Pawn = Cast<AExamCharacter>(InstigatorPawn);
	if (Pawn)
	{
		/* Restore some hitpoints and energy (hunger) */
		Pawn->RestoreCondition(Nutrition * 0.5f, Nutrition);
	}

	Super::OnUsed(InstigatorPawn);
}

