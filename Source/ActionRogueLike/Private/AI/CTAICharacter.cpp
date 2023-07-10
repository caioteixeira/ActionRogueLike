// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTAICharacter.h"

// Sets default values
ACTAICharacter::ACTAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

