// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExamBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API UExamBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void StopLoadingScreen();
};
