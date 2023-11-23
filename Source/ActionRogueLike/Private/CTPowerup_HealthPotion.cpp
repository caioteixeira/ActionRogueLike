// Fill out your copyright notice in the Description page of Project Settings.


#include "CTPowerup_HealthPotion.h"

#include "CTAttributeComponent.h"

ACTPowerup_HealthPotion::ACTPowerup_HealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
}

void ACTPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UCTAttributeComponent* AttributeComponent = Cast<UCTAttributeComponent>(
		InstigatorPawn->GetComponentByClass(UCTAttributeComponent::StaticClass()));
	if(ensure(AttributeComponent) && !AttributeComponent->IsFullHealth())
	{
		if (AttributeComponent->ApplyHealthChange(this, AttributeComponent->GetHealthMax()))
		{
			HideAndCooldownPowerUp();
		}
	}
}
