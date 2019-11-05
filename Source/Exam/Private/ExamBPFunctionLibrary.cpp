// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamBPFunctionLibrary.h"
#include "../Public/ExamLoadingScreen.h"

void UExamBPFunctionLibrary::StopLoadingScreen()
{
	IExamLoadingScreenModule& LoadingScreenModule = IExamLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

void UExamBPFunctionLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	IExamLoadingScreenModule& LoadingScreenModule = IExamLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}