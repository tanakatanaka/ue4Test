// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieCharacter.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;
}

void AZombieCharacter::OnSeePlayer(APawn* Pawn)
{
	if (!IsAlive())
	{
		return;
	}

#if 0
	if (!bSensedTarget)
	{
		BroadcastUpdateAudioLoop(true);
	}

	/* Keep track of the time the player was last sensed in order to clear the target */
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	//AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
	ABaseCharacter* SensedPawn = Cast<ABaseCharacter>(Pawn);
	if (AIController && SensedPawn->IsAlive())
	{
		AIController->SetTargetEnemy(SensedPawn);
	}
#endif
}

void AZombieCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (!IsAlive())
	{
		return;
	}
#if 0
	if (!bSensedTarget)
	{
		BroadcastUpdateAudioLoop(true);
	}

	bSensedTarget = true;
	LastHeardTime = GetWorld()->GetTimeSeconds();

	ASZombieAIController* AIController = Cast<ASZombieAIController>(GetController());
	if (AIController)
	{
		AIController->SetTargetEnemy(PawnInstigator);
	}
#endif
}


void AZombieCharacter::PerformMeleeStrike(AActor* HitActor)
{
#if 0
	if (LastMeleeAttackTime > GetWorld()->GetTimeSeconds() - MeleeStrikeCooldown)
	{
		/* Set timer to start attacking as soon as the cooldown elapses. */
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			// TODO: Set Timer
		}

		/* Attacked before cooldown expired */
		return;
	}

	if (HitActor && HitActor != this && IsAlive())
	{
		ACharacter* OtherPawn = Cast<ACharacter>(HitActor);
		if (OtherPawn)
		{
			APlayerState* MyPS = Cast<APlayerState>(GetPlayerState());
			APlayerState* OtherPS = Cast<APlayerState>(OtherPawn->GetPlayerState());

			if (MyPS && OtherPS)
			{
				if (MyPS->GetTeamNumber() == OtherPS->GetTeamNumber())
				{
					/* Do not attack other zombies. */
					return;
				}

				/* Set to prevent a zombie to attack multiple times in a very short time */
				LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

				FPointDamageEvent DmgEvent;
				DmgEvent.DamageTypeClass = PunchDamageType;
				DmgEvent.Damage = MeleeDamage;

				HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);

				SimulateMeleeStrike();
			}
		}
	}
#endif
}

void AZombieCharacter::SetBotType(EBotBehaviorType NewType)
{
	BotType = NewType;
	/*
	AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
	if (AIController)
	{
		AIController->SetBlackboardBotType(NewType);
	}
	*/
	//BroadcastUpdateAudioLoop(bSensedTarget);
}

UAudioComponent* AZombieCharacter::PlayCharacterSound(USoundCue* CueToPlay)
{
	if (CueToPlay)
	{
		//return UGameplayStatics::SpawnSoundAttached(CueToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}

	return nullptr;
}

void AZombieCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
	Super::PlayHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, bKilled);

	/* Stop playing the hunting sound */
	if (AudioLoopComp && bKilled)
	{
		//AudioLoopComp->Stop();
	}
}