// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAttributeComponent.h"

// Sets default values for this component's properties
UCTAttributeComponent::UCTAttributeComponent()
{
	Health = 150;
	HealthMax = 150;
}

bool UCTAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UCTAttributeComponent::IsFullHealth()
{
	return Health >= HealthMax;
}

float UCTAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

bool UCTAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}
