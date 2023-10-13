// Fill out your copyright notice in the Description page of Project Settings.


#include "CTPowerUpActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ACTPowerUpActor::ACTPowerUpActor()
{
 	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Comp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
}

void ACTPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Define in derived classes
}

void ACTPowerUpActor::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ACTPowerUpActor::HideAndCooldownPowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ACTPowerUpActor::ShowPowerUp, RespawnTime);
}

void ACTPowerUpActor::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	RootComponent->SetVisibility(bNewIsActive, true);
}
