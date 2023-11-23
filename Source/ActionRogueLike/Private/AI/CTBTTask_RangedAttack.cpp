// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTBTTask_RangedAttack.h"

#include "AIController.h"
#include "CTAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UCTBTTask_RangedAttack::UCTBTTask_RangedAttack(): MaxBulletSpread()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type UCTBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		const auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!UCTAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;
		
		const AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
