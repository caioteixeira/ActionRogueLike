// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTPowerUpActor.h"
#include "CTPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ACTPowerup_HealthPotion : public ACTPowerUpActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UMeshComponent* MeshComponent;

	ACTPowerup_HealthPotion();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
