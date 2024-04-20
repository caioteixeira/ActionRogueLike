// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UCTAction_ProjectileAttack::UCTAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void UCTAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void UCTAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		float Radius = 20.0f;
		Shape.SetSphere(Radius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FHitResult Hit;
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FRotator ProjRotation;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params))
		{
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}
		
		const FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
