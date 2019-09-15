
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ExamWeaponInstant.h"
#include "../ExamCharacter.h"
#include "ExamDamageType.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "../Exam.h"

AExamWeaponInstant::AExamWeaponInstant(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	HitDamage = 26;
	WeaponRange = 15000;

	AllowedViewDotHitDir = -1.0f;
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800;
	TracerRoundInterval = 3;
}


void AExamWeaponInstant::SimulateInstantHit(const FVector& ImpactPoint)
{
	const FVector MuzzleOrigin = GetMuzzleLocation();
	const FVector AimDir = (ImpactPoint - MuzzleOrigin).GetSafeNormal();
	const FVector EndTrace = MuzzleOrigin + (AimDir * WeaponRange);
	const FHitResult Impact = WeaponTrace(MuzzleOrigin, EndTrace);

	if (Impact.bBlockingHit)
	{
		SpawnImpactEffects(Impact);
		SpawnTrailEffects(Impact.ImpactPoint);
	}
	else
	{
		SpawnTrailEffects(EndTrace);
	}
}

bool AExamWeaponInstant::ServerNotifyHit_Validate(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir)
{
	return true;
}

void AExamWeaponInstant::SpawnImpactEffects(const FHitResult& Impact)
{
	//if (ImpactTemplate && Impact.bBlockingHit)
	{
		/* This function prepares an actor to spawn, but requires another call to finish the actual spawn progress. This allows manipulation of properties before entering into the level */
		//ASImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<ASImpactEffect>(ImpactTemplate, FTransform(Impact.ImpactPoint.Rotation(), Impact.ImpactPoint));
		//if (EffectActor)
		{
			//EffectActor->SurfaceHit = Impact;
			//UGameplayStatics::FinishSpawningActor(EffectActor, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		}

	}
}

void AExamWeaponInstant::SpawnTrailEffects(const FVector& EndPoint)
{
	BulletsShotCount++;

	const FVector Origin = GetMuzzleLocation();
	FVector ShootDir = EndPoint - Origin;

	if (ShootDir.Size() < MinimumProjectileSpawnDistance)
	{
		return;
	}

	if (BulletsShotCount % TracerRoundInterval == 0)
	{
		if (TracerFX)
		{
			ShootDir.Normalize();
			UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, Origin, ShootDir.Rotation());
		}
	}
	else
	{
		// Only create trails FX by other players.
		AExamCharacter* OwningPawn = GetPawnOwner();
		if (OwningPawn && OwningPawn->IsLocallyControlled())
		{
			return;
		}

		if (TrailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin);
			if (TrailPSC)
			{
				TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
			}
		}
	}
}

void AExamWeaponInstant::FireWeapon()
{
	const FVector AimDir = GetAdjustedAim();
	const FVector CameraPos = GetCameraDamageStartLocation(AimDir);
	const FVector EndPos = CameraPos + (AimDir * WeaponRange);

	/* Check for impact by tracing from the camera position */
	FHitResult Impact = WeaponTrace(CameraPos, EndPos);
	const FVector MuzzleOrigin = GetMuzzleLocation();

	FVector AdjustedAimDir = AimDir;

	if (Impact.bBlockingHit)
	{
		AdjustedAimDir = (Impact.ImpactPoint - MuzzleOrigin).GetSafeNormal();
		Impact = WeaponTrace(MuzzleOrigin, MuzzleOrigin + (AdjustedAimDir * WeaponRange));
	}
	else
	{
		Impact.ImpactPoint = FVector_NetQuantize(EndPos);
	}

	ProcessInstantHit(Impact, MuzzleOrigin, AdjustedAimDir);
}

void AExamWeaponInstant::OnRep_HitLocation()
{
	// Played on all remote clients
	SimulateInstantHit(HitImpactNotify);
}

void AExamWeaponInstant::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ASWeaponInstant, HitImpactNotify, COND_SkipOwner);
}


void AExamWeaponInstant::ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{
	if (MyPawn && MyPawn->IsLocallyControlled() && GetNetMode() == NM_Client)
	{
		// If we are a client and hit something that is controlled by server
		if (Impact.GetActor() && Impact.GetActor()->GetRemoteRole() == ROLE_Authority)
		{
			// Notify the server of our local hit to validate and apply actual hit damage.
			//ServerNotifyHit(Impact, ShootDir);
		}
		else if (Impact.GetActor() == nullptr)
		{
			if (Impact.bBlockingHit)
			{
				//ServerNotifyHit(Impact, ShootDir);
			}
			else
			{
				//ServerNotifyMiss(ShootDir);
			}
		}
	}

	// Process a confirmed hit.
	ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
}

void AExamWeaponInstant::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	float ActualHitDamage = HitDamage;

	/* Handle special damage location on the zombie body (types are setup in the Physics Asset of the zombie */
	UExamDamageType* DmgType = Cast<UExamDamageType>(DamageType->GetDefaultObject());
	UPhysicalMaterial * PhysMat = Impact.PhysMaterial.Get();
	if (PhysMat && DmgType)
	{
		if (PhysMat->SurfaceType == SURFACE_ZOMBIEHEAD)
		{
			ActualHitDamage *= DmgType->GetHeadDamageModifier();
		}
		else if (PhysMat->SurfaceType == SURFACE_ZOMBIELIMB)
		{
			ActualHitDamage *= DmgType->GetLimbDamageModifier();
		}
	}

	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = DamageType;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = ActualHitDamage;

	Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
}


void AExamWeaponInstant::ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{
	// Handle damage
	if (ShouldDealDamage(Impact.GetActor()))
	{
		DealDamage(Impact, ShootDir);
	}

	// Play FX on remote clients
	if (Role == ROLE_Authority)
	{
		HitImpactNotify = Impact.ImpactPoint;
	}

	// Play FX locally
	if (GetNetMode() != NM_DedicatedServer)
	{
		SimulateInstantHit(Impact.ImpactPoint);
	}
}

bool AExamWeaponInstant::ShouldDealDamage(AActor* TestActor) const
{
	// If we are an actor on the server, or the local client has authoritative control over actor, we should register damage.
	if (TestActor)
	{
		if (GetNetMode() != NM_Client ||
			TestActor->Role == ROLE_Authority ||
			TestActor->GetTearOff())
		{
			return true;
		}
	}

	return false;
}