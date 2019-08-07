// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModuleInterface.h"
#include "Modules/ModuleManager.h"

/**
 * 
 */
class EXAM_API IExamLoadingScreenModule : public IModuleInterface
{
public:
	static inline IExamLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IExamLoadingScreenModule>("ExamLoadingScreen");
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;
	virtual void StopInGameLoadingScreen() = 0;
};
