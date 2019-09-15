// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactEffect.generated.h"

class USoundCue;
class UMaterial;


UCLASS()
class EXAM_API AImpactEffect : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AImpactEffect();

	virtual void PostInitializeComponents() override;

	/* FX spawned on standard materials */
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* DefaultFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* PlayerFleshFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ZombieFleshFX;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* DefaultSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* PlayerFleshSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* ZombieFleshSound;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterial* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	float DecalLifeSpan;

	FHitResult SurfaceHit;


protected:
	UParticleSystem* GetImpactFX(EPhysicalSurface SurfaceType) const;
	USoundCue* GetImpactSound(EPhysicalSurface SurfaceType) const;
};
