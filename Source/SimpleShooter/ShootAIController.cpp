// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootAIController.h"

#include "ShooterCharacter.h"
#include "GameFramework/Character.h"



AShootAIController::AShootAIController()
{
	
}

void AShootAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShootAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AShootAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if(ControlledCharacter != nullptr)
	{
		return	ControlledCharacter->IsDead();
	}

	return true;
}

