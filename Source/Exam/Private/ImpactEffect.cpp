// Fill out your copyright notice in the Description page of Project Settings.

#include"../Exam.h"
#include "ImpactEffect.h"
#include "Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Materials/Material.h"

// Sets default values
AImpactEffect::AImpactEffect()
{
	bAutoDestroyWhenFinished = true;
	PrimaryActorTick.bCanEverTick = true;

	DecalLifeSpan = 10.0f;
	DecalSize = 16.0f;
}



void AImpactEffect::PostInitializeComponents()
{
	//Super::PostInitializeComponents();

	/* Figure out what we hit (SurfaceHit is setting during actor instantiation in weapon class) */
	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (DecalMaterial)
	{
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(DecalSize, DecalSize, 1.0f),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			DecalLifeSpan);
	}
}


UParticleSystem* AImpactEffect::GetImpactFX(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType)
	{
	case SURFACE_DEFAULT:
		return DefaultFX;
	case SURFACE_FLESH:
		return PlayerFleshFX;
	case SURFACE_ZOMBIEBODY:
	case SURFACE_ZOMBIEHEAD:
	case SURFACE_ZOMBIELIMB:
		return ZombieFleshFX;
	default:
		return nullptr;
	}
}


USoundCue* AImpactEffect::GetImpactSound(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType)
	{
	case SURFACE_DEFAULT:
		return DefaultSound;
	case SURFACE_FLESH:
		return PlayerFleshSound;
	case SURFACE_ZOMBIEBODY:
	case SURFACE_ZOMBIEHEAD:
	case SURFACE_ZOMBIELIMB:
		return ZombieFleshSound;
	default:
		return nullptr;
	}
}