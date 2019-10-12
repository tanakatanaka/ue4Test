// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ExamTypes.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class EXAM_API AZombieCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();
	
	
	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
	bool bIsPunching;

	/* The bot behavior we want this bot to execute, (passive/patrol) by specifying EditAnywhere we can edit this value per-instance when placed on the map. */
	UPROPERTY(EditAnywhere, Category = "AI")
	EBotBehaviorType BotType;

	/* The thinking part of the brain, steers our zombie and makes decisions based on the data we feed it from the Blackboard */
	/* Assigned at the Character level (instead of Controller) so we may use different zombie behaviors while re-using one controller. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/* Change default bot type during gameplay */
	void SetBotType(EBotBehaviorType NewType);

protected:
	
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);


	UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);

	UFUNCTION(BlueprintCallable, Category = "Attacking")
	void PerformMeleeStrike(AActor* HitActor);
	
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundPlayerNoticed;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundHunting;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundWandering;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundAttackMelee;

	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	/* Minimum time between melee attacks */
	float MeleeStrikeCooldown;

	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLoopComp;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	TSubclassOf<UDamageType> PunchDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float MeleeDamage;


private:
	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//class UPawnSensingComponent* PawnSensingComp;

	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	/* Last time we attacked something */
	float LastMeleeAttackTime;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	bool bSensedTarget;

};
