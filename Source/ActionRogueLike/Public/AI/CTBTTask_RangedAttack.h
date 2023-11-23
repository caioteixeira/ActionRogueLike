// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CTBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UCTBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxBulletSpread;

public:
	explicit UCTBTTask_RangedAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
