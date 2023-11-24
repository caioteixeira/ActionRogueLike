// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTAICharacter.generated.h"

class UCTWorldUserWidget;
class UCTAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ACTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTAICharacter();

protected:
	UPROPERTY(BlueprintReadOnly)
	UCTWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCTAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	FName TimeToHitParameterName;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UCTWorldUserWidget> HealthBarWidgetClass;

	virtual void PostInitializeComponents() override;
	void SetTargetActor(AActor* Target) const;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth, float Delta);
};
