// Fill out your copyright notice in the Description page of Project Settings.


#include "CTExplosiveBarrel.h"

#include "CTAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
ACTExplosiveBarrel::ACTExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(MeshComponent);
	RadialForceComponent->SetAutoActivate(false);

	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->ImpulseStrength = 400.f;
	RadialForceComponent->bImpulseVelChange = true;
}

void ACTExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComponent->OnComponentHit.AddDynamic(this, &ACTExplosiveBarrel::OnHit);
}

void ACTExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Barrel was hit"));

	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	const FString CombinedString = FString::Printf(TEXT("Hit at location %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	auto* AttributeComponent = UCTAttributeComponent::GetAttributeComponent(OtherActor);
	if (IsValid(AttributeComponent))
	{
		AttributeComponent->ApplyHealthChange(this, -10.f);
	}
}

// Called every frame
void ACTExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

