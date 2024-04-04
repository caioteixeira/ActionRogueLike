// Fill out your copyright notice in the Description page of Project Settings.


#include "CTGameModeBase.h"

#include "CTAttributeComponent.h"
#include "CTCharacter.h"
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

void ACTGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ACTCharacter* Player = Cast<ACTCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		constexpr float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ACTGameModeBase::KillAll()
{
	for (TActorIterator<ACTAICharacter> It(GetWorld()); It; ++It)
	{
		ACTAICharacter* Bot = *It;
		UCTAttributeComponent* AttributeComponent = UCTAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @fixme: pass player for kill credit
		}
	}
}

void ACTGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ACTAICharacter> It(GetWorld()); It; ++It)
	{
		ACTAICharacter* Bot = *It;
		const UCTAttributeComponent* AttributeComponent = UCTAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultCurve)
	{
		MaxBotCount = DifficultCurve->GetFloatValue(GetWorld()->TimeSeconds);	
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum capacity, skipping bot spawn."));

		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACTGameModeBase::OnQueryCompleted);
	}
}

void ACTGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
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
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}
