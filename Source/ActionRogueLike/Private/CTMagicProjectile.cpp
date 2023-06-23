// Fill out your copyright notice in the Description page of Project Settings.


#include "CTMagicProjectile.h"

#include "CTAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACTMagicProjectile::ACTMagicProjectile()
{
 	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACTMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;
}

void ACTMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UCTAttributeComponent* AttributeComponent = Cast<UCTAttributeComponent>(
			OtherActor->GetComponentByClass(UCTAttributeComponent::StaticClass()));
		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-DamageAmount);

			Explode();
		}
		
	}
}

