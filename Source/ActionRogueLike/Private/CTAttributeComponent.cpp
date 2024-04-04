// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAttributeComponent.h"

#include "CTGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f,
	TEXT("Global damage multiplier for attribute component"), ECVF_Cheat);


// Sets default values for this component's properties
UCTAttributeComponent::UCTAttributeComponent()
{
	Health = 150;
	HealthMax = 150;
}

bool UCTAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
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
	if (Delta < 0.0f && !GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	const float ActualDelta = Health - OldHealth;

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ACTGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACTGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return ActualDelta != 0;
}

UCTAttributeComponent* UCTAttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (IsValid(FromActor))
	{
		return FromActor->FindComponentByClass<UCTAttributeComponent>();
	}

	return nullptr;
}

bool UCTAttributeComponent::IsActorAlive(AActor* Actor)
{
	const UCTAttributeComponent* AttributeComponent = GetAttributeComponent(Actor);
	if (IsValid(AttributeComponent))
	{
		return AttributeComponent->IsAlive();
	}

	return false;  
}
