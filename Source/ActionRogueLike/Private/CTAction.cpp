// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAction.h"

#include "CTActionComponent.h"

void UCTAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UCTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop: %s"), *GetNameSafe(this));
}

UWorld* UCTAction::GetWorld() const
{
	// Outer is set when action is created
	if (UCTActionComponent* Comp = Cast<UCTActionComponent>(GetOuter()))
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
