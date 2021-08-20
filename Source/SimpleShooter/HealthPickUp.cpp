// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUp.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

// Sets default values
AHealthPickUp::AHealthPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	HpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hp Mesh"));
	HpMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();
	FindPressurePlate();

	ActorThatHeals = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void AHealthPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatHeals))
	{
		Cast<AShooterCharacter>(ActorThatHeals)->Heal(HpValue);
		Destroy();
	}
}

void AHealthPickUp::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s actor has no PressurePlate set!"), *GetOwner()->GetName());
	}
}