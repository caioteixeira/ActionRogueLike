// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "CTAttributeComponent.h"
#include "AI/CTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ACTAICharacter::ACTAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComponent = CreateDefaultSubobject<UCTAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACTAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ACTAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACTAICharacter::OnHealthChanged);
}

void ACTAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (AIC)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();

		BBComponent->SetValueAsObject("TargetActor", Pawn);

		//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER_SPOTTED", nullptr, FColor::Blue);
	}
}

void ACTAICharacter::OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		

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

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
