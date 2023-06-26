// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAttributeComponent.h"

// Sets default values for this component's properties
UCTAttributeComponent::UCTAttributeComponent()
{
	Health = 100;
}

bool UCTAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UCTAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}
