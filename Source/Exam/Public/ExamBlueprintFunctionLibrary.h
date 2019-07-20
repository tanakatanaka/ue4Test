// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExamBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_API UExamBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable, Category = Loading)
	static void StopLoadingScreen();

};
