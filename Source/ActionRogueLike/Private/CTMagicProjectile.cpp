// Fill out your copyright notice in the Description page of Project Settings.


#include "CTMagicProjectile.h"

#include "CTAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACTMagicProjectile::ACTMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACTMagicProjectile::OnActorOverlap);

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ACTMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UCTAttributeComponent* AttributeComponent = Cast<UCTAttributeComponent>(
			OtherActor->GetComponentByClass(UCTAttributeComponent::StaticClass()));
		if (AttributeComponent && OtherActor != GetInstigator())
		{
			AttributeComponent->ApplyHealthChange(-20.0f);

			Destroy();
		}
		
	}
}

// Called when the game starts or when spawned
void ACTMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

