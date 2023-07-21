// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ACTAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nulltpr!, Please assign BehaviorTree in your AI controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}*/
}
