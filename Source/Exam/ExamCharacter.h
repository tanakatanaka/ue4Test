// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "../Public/BaseCharacter.h"
#include "ExamCharacter.generated.h"

class AExamWeapon;
class UCarryObjectComponent;

UCLASS(config=Game)
class AExamCharacter : public ABaseCharacter
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

	virtual void BeginPlay() override;

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


	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	bool WeaponSlotAvailable(EInventorySlot CheckSlot);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	void RestoreCondition(float HealthRestored, float HungerRestored);

	void SetCurrentWeapon(class AExamWeapon* newWeapon, class AExamWeapon* LastWeapon = nullptr);

	void RemoveWeapon(class AExamWeapon* Weapon, bool bDestroy);

	class AExamWeapon* PreviousWeapon;
	class AExamWeapon* CurrentWeapon;

	UPROPERTY(Transient, Replicated)
	TArray<AExamWeapon*> Inventory;

protected:

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

	virtual void PawnClientRestart() override;


	void SetTargeting(bool NewTargeting);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MakePawnNoise(float Loudness);

	void AddWeapon(class AExamWeapon* Weapon);

	bool CanFire() const;
	bool CanReload();
	bool IsAlive();

	void OnReload();
	void OnStartFire();
	void OnStopFire();
	void StartFire();
	void OnNextWeapon();
	void OnPrevWeapon();
	void StopFire();
	void OnStartTargeting();
	void OnEndTargeting();
	void StopWeaponFire();
	void OnToggleCarryActor();

	/* Mapped to input. Drops current weapon */
	void DropWeapon();

	class UCarryObjectComponent* CarriedObjectComp;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsFiring() const;

	/* Use the usable actor currently in focus, if any */
	void Use();

	class AUsableActor* GetUsableInView();

	/*Max distance to use/focus on actors. */
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float DropWeaponMaxDistance;
	
	UStaticMeshComponent* GetCarriedMeshComp();

	float LastNoiseLoudness;

	float LastMakeNoiseTime;

	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint;

	/* Attachpoint for items carried on the belt/pelvis. */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName PelvisAttachPoint;

	/* Attachpoint for primary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SpineAttachPoint;

	/* Damage type applied when player suffers critical hunger */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	TSubclassOf<UDamageType> m_HungerDamageTyp;
};

