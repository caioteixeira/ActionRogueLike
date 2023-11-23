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

bool UCTAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	const float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return OldHealth != 0;
}
