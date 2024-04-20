// Fill out your copyright notice in the Description page of Project Settings.


#include "CTActionComponent.h"

#include "CTAction.h"


UCTActionComponent::UCTActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCTActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCTAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UCTActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCTActionComponent::AddAction(TSubclassOf<UCTAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UCTAction* NewAction = NewObject<UCTAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UCTActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UCTAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCTActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UCTAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
