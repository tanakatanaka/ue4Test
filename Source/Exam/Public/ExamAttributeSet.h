// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExamTypes.h"
#include "UObject/NoExportTypes.h"
#include "ExamAttributeSet.generated.h"

class AExamWeapon;
class AExamCharacter;

/**
 * 
 */
UCLASS()
class EXAM_API UExamAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	UExamAttributeSet();
	//void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue

	bool IsAlive();

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	float AttackPower;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	float DefensePower;

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	float MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	float Damage;

	void AdjustAttributeForMaxChange();
	void SetCurrentWeapon(AExamCharacter* Owner, class AExamWeapon* newWeapon, class AExamWeapon* LastWeapon = nullptr);
	FName GetInventoryAttachPoint(EInventorySlot Slot) const;

	bool bIsEquipped;
	bool bWantsToFire;

	class AExamWeapon* PreviousWeapon;
	class AExamWeapon* CurrentWeapon;

private:
	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint;

	/* Attachpoint for items carried on the belt/pelvis. */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName PelvisAttachPoint;

	/* Attachpoint for primary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SpineAttachPoint;
};