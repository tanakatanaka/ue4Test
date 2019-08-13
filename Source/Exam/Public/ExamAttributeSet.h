// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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


	bool bIsEquipped;
	bool bWantsToFire;

	class AExamWeapon* PreviousWeapon;
	class AExamWeapon* CurrentWeapon;
};