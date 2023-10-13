// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CTPowerUpActor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ACTPowerUpActor : public AActor, public ICTGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:
	UPROPERTY(EditAnywhere, Category="PowerUp");
	float RespawnTime;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	ACTPowerUpActor();

protected:
	virtual void ShowPowerUp();
	virtual void HideAndCooldownPowerUp();
	virtual void SetPowerUpState(bool bNewIsActive);

private:
	FTimerHandle TimerHandle_RespawnTimer;

};
