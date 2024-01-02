// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CTBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UCTBTService_CheckLowHealth : public UBTService
{
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector LowHealthKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	GENERATED_BODY()
};
