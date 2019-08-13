// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExamTypes.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "ExamWeapon.generated.h"

class UAnimMontage;
class USoundCue;
class AExamCharacter;
class UAudioComponent;

UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Equipping,
	Reloading
};

UCLASS(ABSTRACT, Blueprintable)
class EXAM_API AExamWeapon : public AActor
{
	GENERATED_BODY()

public:

	virtual void OnUnEquip();
	void OnEquip(bool bPlayAnimation);

	void StartFire();
	void StopFire();
	void SetOwningPawn(AExamCharacter* NewOwner);
	void AttachMeshToPawn(EInventorySlot Slot = EInventorySlot::Hands);

protected:

	AExamWeapon(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnRep_Reload();
	void StopWeaponAnimation(UAnimMontage* Animation);
	/* Is weapon and character currently capable of starting a reload */
	bool CanReload();
	bool CanFire() const;

	virtual void SimulateWeaponFire();

	class AExamCharacter* MyPawn;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float NoEquipAnimDuration;

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UPROPERTY(EditDefaultsOnly)
	int32 StartAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoPerClip;

	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmo;

	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadAnim;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	bool bPendingReload;
	
	virtual void OnEquipFinished();

	void DetachMeshFromPawn();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* OutOfAmmoSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* EquipSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAnim;

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	virtual void HandleFiring();
	virtual void PostInitializeComponents() override;
	virtual void StopSimulatingWeaponFire();

	void DetermineWeaponState();
	void SetWeaponState(EWeaponState NewState);
	void OnBurstStarted();
	void OnBurstFinished();

	float TimeBetweenShots; //Time between shots for repeating fire
	float EquipStartedTime;
	float EquipDuration;
	float LastFireTime;
	
	int32 BurstCounter;

	bool bIsEquipped;
	bool bPendingEquip;
	bool bWantsToFire;
	bool bRefiring;
	bool bPlayingFireAnim;

	EWeaponState CurrentState;
	FTimerHandle TimerHandle_ReloadWeapon;
	FTimerHandle TimerHandle_HandleFiring;
	FTimerHandle EquipFinishedTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ShotsPerMinute;

	virtual void StartReload(bool bFromReplication = false);
	virtual void StopSimulateReload();

};
