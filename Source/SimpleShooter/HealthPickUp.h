// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "HealthPickUp.generated.h"

UCLASS()
class SIMPLESHOOTER_API AHealthPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickUp();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		AActor* ActorThatHeals = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void FindPressurePlate();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HpMesh;

	UPROPERTY(EditAnywhere)
	float HpValue = 300;
};
