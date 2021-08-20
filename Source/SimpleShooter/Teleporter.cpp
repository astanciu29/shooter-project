// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleShooter/Characters/ShooterCharacter.h"
#include "Teleporter.h"

// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	TeleporterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Teleporter Mesh"));
	TeleporterMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();

	FindPressurePlate();

	ActorThatTeleports = GetWorld()->GetFirstPlayerController()->GetPawn();	
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the actor is pressing on the plate, teleport
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatTeleports))
	{
		Cast<AShooterCharacter>(ActorThatTeleports)->Teleport(GetTeleportationCoordinates(), GetRotationCoordinates());
	}
}

void ATeleporter::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s actor has no PressurePlate set!"), *GetOwner()->GetName());
	}
}

//If outside of the healing room, teleport inside; If in the healing room, teleport outside of it.
FVector ATeleporter::GetTeleportationCoordinates()
{
	return isInHealRoom ? TeleportationCoordinatesHallway : TeleportationCoordinatesHealRoom;
}

FRotator ATeleporter::GetRotationCoordinates()
{
	return isInHealRoom ? TeleportationRotationHallway : TeleportationRotationHealRoom;
}