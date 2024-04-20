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
	
	void PrimaryAttack();
	void BlackHoleAttack();
	void Dash();

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
