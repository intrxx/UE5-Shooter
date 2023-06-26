// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShootAIController.h"
#include "GameFramework/Controller.h"

void AKillEmAllGameMode::PawnKilled(APawn* CalledPawnKilled)
{
	Super::PawnKilled(CalledPawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(CalledPawnKilled->GetController());
	
	if(PlayerController != nullptr)
	{
		EndGame(false);
	}

	for(AShootAIController* AIController : TActorRange<AShootAIController>(GetWorld()))
	{
		if(!AIController->IsDead())
		{
			return;
		}
	}
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}