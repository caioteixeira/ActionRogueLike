// Fill out your copyright notice in the Description page of Project Settings.


#include "CTItemChest.h"
#include "Components/StaticMeshComponent.h"

void ACTItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
}

// Sets default values
ACTItemChest::ACTItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetPitch = 110;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ACTItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

