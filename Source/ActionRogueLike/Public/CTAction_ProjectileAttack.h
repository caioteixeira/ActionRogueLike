// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTAction.h"
#include "CTAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UCTAction_ProjectileAttack : public UCTAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack");
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:
	UCTAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
