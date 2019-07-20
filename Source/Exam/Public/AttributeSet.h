// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Templates/SubclassOf.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Actor.h"
#include "Engine/CurveTable.h"
#include "Engine/DataTable.h"
#include "AttributeSet.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
struct FGameplayAbilityActorInfo;
struct FAggregator;

USTRUCT(BlueprintType)
struct FGameplayAttributeData 
{
	GENERATED_USTRUCT_BODY()

	FGameplayAttributeData()
		: BaseValue(0.f)
		, CurrentValue(0.f)
	{}

	FGameplayAttributeData(float DefaultValue)
		: BaseValue(DefaultValue)
		, CurrentValue(DefaultValue)
	{}

	virtual ~FGameplayAttributeData()
	{}

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float BaseValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurrentValue;

};
