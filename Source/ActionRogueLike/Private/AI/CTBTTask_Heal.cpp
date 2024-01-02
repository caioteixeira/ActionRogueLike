// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTBTTask_Heal.h"

#include "AIController.h"
#include "CTAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UCTBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* Pawn = Cast<ACharacter>(MyController->GetPawn());
		if (!IsValid(Pawn))
		{
			return EBTNodeResult::Failed;
		}

		UCTAttributeComponent* AttributeComponent = UCTAttributeComponent::GetAttributeComponent(Pawn);

		AttributeComponent->ApplyHealthChange(Pawn, AttributeComponent->GetHealthMax());
	}
	
	return EBTNodeResult::Failed;
}
