// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Public/ExamAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "ExamCharacter.generated.h"


class UCarryObjectComponent;

UCLASS(config=Game)
class AExamCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AExamCharacter();

	void SetCurrentWeapon(class AExamWeapon* NewWeapon, class AExamWeapon* LastWeapon = nullptr);

	void EquipWeapon(AExamWeapon* Weapon);

	UPROPERTY(Transient, Replicated)
	bool bIsJumping;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	FName GetInventoryAttachPoint(EInventorySlot Slot) const;

	FORCEINLINE UCameraComponent* GetCameraComponent()
	{
		return FollowCamera;
	}

	/* Is player aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool IsTargeting() const;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsSprinting() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	bool WeaponSlotAvailable(EInventorySlot CheckSlot);

protected:

	/* Character wants to run, checked during Tick to see if allowed */
	UPROPERTY(Transient, Replicated)
	bool bWantsToRun;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/* Called every frame */
	//virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	UExamAttributeSet* AttributeSet;

	virtual void PawnClientRestart() override;


	UPROPERTY(Transient, Replicated)
	bool bIsTargeting;

	void SetTargeting(bool NewTargeting);

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;
	
	void AddWeapon(class AExamWeapon* Weapon);

	bool CanFire() const;
	bool CanReload() const;
	bool IsAlive() const;

	void OnStartFire();
	void OnStopFire();
	void StartFire();
	void StopFire();
	void OnStartTargeting();
	void OnEndTargeting();
	void StopWeaponFire();

	class UCarryObjectComponent* CarriedObjectComp;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsFiring() const;

private:
	UStaticMeshComponent* GetCarriedMeshComp();
};

