// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Public/ExamWeapon.h"
#include "GameFramework/Actor.h"
#include "ExamWeaponInstant.generated.h"

UCLASS()
class EXAM_API AExamWeaponInstant : public AExamWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float WeaponRange;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_HitLocation)
	FVector HitImpactNotify;

	UFUNCTION()
	void OnRep_HitLocation();

private:
	void SimulateInstantHit(const FVector& ImpactPoint);
	void SpawnImpactEffects(const FHitResult& Impact);
	void SpawnTrailEffects(const FVector& EndPoint);

	bool ServerNotifyHit_Validate(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir);

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class ASImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly)
	FName TrailTargetParam;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TracerFX;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class ASImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly)
	float MinimumProjectileSpawnDistance;

	UPROPERTY(EditDefaultsOnly)
	int32 TracerRoundInterval;

	int32 BulletsShotCount;
};
