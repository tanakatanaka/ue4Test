
// Fill out your copyright notice in the Description page of Project Settings.
#include "UsableActor.h"
#include "UObject/UObjectGlobals.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUsableActor::AUsableActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
}

void AUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void AUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}

void AUsableActor::OnUsed(APawn* InstigatorPawn)
{
	// Nothing to do here...
}
