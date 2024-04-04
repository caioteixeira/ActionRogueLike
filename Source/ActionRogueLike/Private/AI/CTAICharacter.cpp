// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "CTAttributeComponent.h"
#include "CTWorldUserWidget.h"
#include "AI/CTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ACTAICharacter::ACTAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComponent = CreateDefaultSubobject<UCTAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParameterName = "TimeToHit";
}

void ACTAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ACTAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACTAICharacter::OnHealthChanged);
}

void ACTAICharacter::SetTargetActor(AActor* Target) const
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (AIC)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		BBComponent->SetValueAsObject("TargetActor", Target);
	}
}

void ACTAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER_SPOTTED", nullptr, FColor::Blue);
}

void ACTAICharacter::OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth,
                                     float Delta)
{
	if (Delta < 0.0f)
	{
		if (IsValid(InstigatorActor) && InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (!IsValid(ActiveHealthBar))
		{
			ActiveHealthBar = CreateWidget<UCTWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

			if (IsValid(ActiveHealthBar))
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameterName, GetWorld()->TimeSeconds);

		if (newHealth <= 0.0f)
		{
			// stop BT
			const AAIController* AIController = Cast<ACTAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}
			
			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
