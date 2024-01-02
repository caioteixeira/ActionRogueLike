// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTBTService_CheckLowHealth.h"

#include "AIController.h"
#include "CTAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCTBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!(ensure(BlackboardComponent)))
	{
		return;
	}

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!(ensure(AIController)))
	{
		return;
	}
	
	APawn* AIPawn = AIController->GetPawn();
	if (!(ensure(AIPawn)))
	{
		return;
	}

	const UCTAttributeComponent* AttributeComponent = UCTAttributeComponent::GetAttributeComponent(AIPawn);
	if (ensure(AttributeComponent))
	{
		const bool bHasLowHealth = AttributeComponent->Health / AttributeComponent->HealthMax < 0.5f;
		BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, bHasLowHealth);
	}
}
