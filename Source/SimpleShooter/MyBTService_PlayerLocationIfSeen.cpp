// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


UMyBTService_PlayerLocationIfSeen::UMyBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location if Seen");
}

void UMyBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if(AIOwner == nullptr)
	{
		return;
	}
	
	if(AIOwner->LineOfSightTo(PlayerCharacter,FVector(ForceInit)))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"),PlayerCharacter->GetActorLocation());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}
