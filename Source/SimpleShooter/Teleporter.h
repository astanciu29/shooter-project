// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Teleporter.generated.h"

UCLASS()
class SIMPLESHOOTER_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		AActor* ActorThatTeleports = nullptr;

	UPROPERTY(EditAnywhere)
		bool isInHealRoom = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void FindPressurePlate();
	FVector GetTeleportationCoordinates();
	FRotator GetRotationCoordinates();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TeleporterMesh;

	UPROPERTY(EditAnywhere)
	FVector TeleportationCoordinatesHealRoom = FVector(4720.0f, -6530.0f, 87.999985f);

	UPROPERTY(EditAnywhere)
	FRotator TeleportationRotationHealRoom = FRotator(0.0f, 170.000198f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector TeleportationCoordinatesHallway = FVector(4201.916504f, -6390.0f,  100.0f);

	UPROPERTY(EditAnywhere)
	FRotator TeleportationRotationHallway = FRotator(0.0f, 180.000198f, 0.0f);
};
