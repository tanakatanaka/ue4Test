// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "ExamDamageType.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API UExamDamageType : public UDamageType
{
	GENERATED_BODY()
	
	UExamDamageType();

	/* Can player die from this damage type (eg. players don't die from hunger) */
	UPROPERTY(EditDefaultsOnly)
	bool bCanDieFrom;

	/* Damage modifier for headshot damage */
	UPROPERTY(EditDefaultsOnly)
	float HeadDmgModifier;

	UPROPERTY(EditDefaultsOnly)
	float LimbDmgModifier;

public:
	bool GetCanDieFrom();

	float GetHeadDamageModifier();

	float GetLimbDamageModifier();
};
