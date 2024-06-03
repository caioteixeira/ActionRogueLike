// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAction.h"

#include "CTActionComponent.h"

void UCTAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UCTActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UCTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop: %s"), *GetNameSafe(this));

	UCTActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.RemoveTags(GrantsTags);
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