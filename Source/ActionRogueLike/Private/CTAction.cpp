// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAction.h"

#include "CTActionComponent.h"

bool UCTAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	UCTActionComponent* Component = GetOwningComponent();
	
	if (Component->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UCTAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UCTActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UCTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	UCTActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool UCTAction::IsRunning_Implementation()
{
	return bIsRunning;
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

UCTActionComponent* UCTAction::GetOwningComponent() const
{
	return Cast<UCTActionComponent>(GetOuter());
}
