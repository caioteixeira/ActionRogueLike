// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTCharacter.generated.h"

class UCTActionComponent;
class UCTAttributeComponent;
class UCTInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ACTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTCharacter();

protected:
	UPROPERTY(EditAnywhere, Category="Attack");
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack");
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack");
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_Dash;

	FTimerHandle TimerHandle_BlackHoleAttack;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UCTInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCTAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCTActionComponent* ActionComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void SprintStart();
	void SprintStop();
	
	void SpawnProjectile(TSubclassOf<AActor>);
	void PrimaryAttack();
	void PrimaryAttack_TimerElapsed();
	void BlackHoleAttack();
	void BlackHoleAttack_TimerElapsed();
	void Dash();
	void Dash_TimerElapsed();

	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
