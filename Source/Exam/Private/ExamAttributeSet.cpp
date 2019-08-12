// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamAttributeSet.h"

UExamAttributeSet::UExamAttributeSet()
{
	Health = 1.f;
	MaxHealth = 1.f;
	AttackPower = 1.f;
	DefensePower = 1.f;
	MoveSpeed = 1.f;
	Damage = 1.f;
}

bool UExamAttributeSet::IsAlive()
{
	return Health > 0;
}


void UExamAttributeSet::AdjustAttributeForMaxChange()
{

}