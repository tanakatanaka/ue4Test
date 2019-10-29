// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETypes.h"
#include "BaseCharacter.h"
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

	void RestoreCondition(float HealthRestored, float HungerRestored);

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

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Hunger")
	float Hunger;

	UPROPERTY(BlueprintReadOnly, Category = "Hunger")
	float MaxHunger;

	void AdjustAttributeForMaxChange();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool bIsEquipped;
	bool bWantsToFire;

};