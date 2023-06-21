// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "CTTargetDummy.generated.h"

class UCTAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ACTTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	UCTAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth, float Delta);
};
