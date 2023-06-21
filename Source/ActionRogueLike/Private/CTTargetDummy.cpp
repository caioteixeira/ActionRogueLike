// Fill out your copyright notice in the Description page of Project Settings.


#include "CTTargetDummy.h"

// Sets default values
ACTTargetDummy::ACTTargetDummy()
{
 	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComponent;

	AttributeComponent = CreateDefaultSubobject<UCTAttributeComponent>("AttributeComp");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACTTargetDummy::OnHealthChanged);
}

void ACTTargetDummy::OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
