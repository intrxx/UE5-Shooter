// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUD;
};
