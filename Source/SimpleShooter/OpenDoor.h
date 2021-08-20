// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindPressurePlate();
	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float InitialRoll;
	float CurrentRoll;

	UPROPERTY(EditAnywhere)
		float OpenHeight = 350.f;

	float DoorLastOpen = 0.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 5.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens = nullptr;

	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;		
};
