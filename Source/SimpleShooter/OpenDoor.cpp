// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//Initial coordinates of the door
	InitialRoll = GetOwner()->GetActorRotation().Roll;
	CurrentRoll = InitialRoll;
	OpenHeight += CurrentRoll;

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindPressurePlate();
	FindAudioComponent();	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the actor is pressing on the Pressure Plate associated to the door, Open door
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		//If the the actor is no longer pressig on the Pressure Plate associated to the door, Close door
		if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentRoll = FMath::Lerp(CurrentRoll, OpenHeight, DeltaTime * DoorOpenSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = CurrentRoll;
	GetOwner()->SetActorLocation(DoorLocation);

	if (!AudioComponent)
		return;

	CloseDoorSound = false;
	if (!OpenDoorSound) {
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentRoll = FMath::Lerp(CurrentRoll, InitialRoll, DeltaTime * DoorCloseSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = CurrentRoll;
	GetOwner()->SetActorLocation(DoorLocation);

	if (!AudioComponent)
		return;

	OpenDoorSound = false;

	if (!CloseDoorSound) {
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component."), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s actor has the OpenDoor component on it, but no PressurePlate set!"), *GetOwner()->GetName());
	}
}