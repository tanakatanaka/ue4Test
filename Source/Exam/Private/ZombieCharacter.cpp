#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Public/ExamAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	AttributeSet = CreateDefaultSubobject<UExamAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->Initialize(Cast<ABaseCharacter>(this));

	MyAI = CreateDefaultSubobject<AZombieAIController>(TEXT("ZombieAIController"));
	//MyAI->ForcedHold(this);
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AZombieCharacter::OnHearNoise);
	}
	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnMeleeCompBeginOverlap);
	}

	//BroadcastUpdateAudioLoop(bSensedTarget);

	/* Assign a basic name to identify the bots in the HUD. */
	/*
	APlayerState* PS = Cast<APlayerState>(GetPlayerState());
	if (PS)
	{
		PS->SetPlayerName("Bot");
		PS->bIsABot = true;
	}
	*/
}

void AZombieCharacter::Tick(float DeltaSeconds)
{
	return;

	Super::Tick(DeltaSeconds);

	/* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
	//if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut
	//	&& (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
	{
		AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetTargetEnemy(nullptr);


			/* Stop playing the hunting sound */
			//BroadcastUpdateAudioLoop(false);
		}
	}
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



void AZombieCharacter::OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	PerformMeleeStrike(OtherActor);

	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AZombieCharacter::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void AZombieCharacter::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ABaseCharacter* OverlappingPawn = Cast<ABaseCharacter>(Overlaps[i]);
		if (OverlappingPawn)
		{
			PerformMeleeStrike(OverlappingPawn);
			//break; /* Uncomment to only attack one pawn maximum */
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
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