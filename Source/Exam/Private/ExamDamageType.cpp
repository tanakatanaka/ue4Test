// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamDamageType.h"

UExamDamageType::UExamDamageType()
{
	/* We apply this modifier based on the physics material setup to the head of the enemy PhysAsset */
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;

	bCanDieFrom = true;
}

bool UExamDamageType::GetCanDieFrom()
{
	return bCanDieFrom;
}


float UExamDamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UExamDamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}
