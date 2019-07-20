// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ExamGameModeBase.h"
#include "../Public/ExamStateBase.h"
#include "../Public/ExamPlayerController.h"

AExamGameModeBase::AExamGameModeBase()
{
	GameStateClass = AExamStateBase::StaticClass();
	PlayerControllerClass = AExamPlayerController::StaticClass();
}

