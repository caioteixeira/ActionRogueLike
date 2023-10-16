// Fill out your copyright notice in the Description page of Project Settings.


#include "CTGameModeBase.h"

#include "CTAttributeComponent.h"
#include "EngineUtils.h"
#include "AI/CTAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ACTGameModeBase::ACTGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ACTGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(
		TimerHandle_SpawnBots, this, &ACTGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ACTGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACTGameModeBase::OnQueryCompleted);
	}
}

void ACTGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                       EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query failed!"));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ACTAICharacter> It(GetWorld()); It; ++It)
	{
		ACTAICharacter* Bot = *It;

		UCTAttributeComponent* AttributeComponent = Cast<UCTAttributeComponent>(
			Bot->GetComponentByClass(UCTAttributeComponent::StaticClass()));
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;
	if (DifficultCurve)
	{
		MaxBotCount = DifficultCurve->GetFloatValue(GetWorld()->TimeSeconds);	
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
