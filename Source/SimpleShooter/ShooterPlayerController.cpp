// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

 void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus , bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	UE_LOG(LogTemp, Warning, TEXT("Game ended"));
}