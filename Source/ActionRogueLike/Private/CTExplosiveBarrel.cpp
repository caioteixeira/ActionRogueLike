// Fill out your copyright notice in the Description page of Project Settings.


#include "CTExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ACTExplosiveBarrel::ACTExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
}

// Called when the game starts or when spawned
void ACTExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentHit.AddDynamic(this, &ACTExplosiveBarrel::OnHit);
}

void ACTExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Barrel was hit"));

	RadialForceComponent->FireImpulse();
}

// Called every frame
void ACTExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

