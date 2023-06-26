// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(HUDClass != nullptr)
	{
		HUD = CreateWidget(this, HUDClass);
		HUD->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();
	if(bIsWinner)
	{
		if(WinWidgetClass != nullptr)
		{
			UUserWidget* WinScreen = CreateWidget(this, WinWidgetClass);
		
			if(WinScreen != nullptr)
			{
				WinScreen->AddToViewport();
			}
		}
	}
	else
	{
		if(LoseWidgetClass != nullptr)
		{
			UUserWidget* LoseScreen = CreateWidget(this, LoseWidgetClass);
		
			if(LoseScreen != nullptr)
			{
				LoseScreen->AddToViewport();
			}
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel,RestartDelay);

	
}
