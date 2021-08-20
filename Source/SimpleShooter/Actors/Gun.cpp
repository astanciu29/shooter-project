// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	//Create the mesh of the Gun
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Returns the owner of the Gun
AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

//Calculates the trajectory of the shooting path
bool AGun::GunTrace(FHitResult& Hit, FVector& ShootDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShootDirection = -Rotation.Vector();

	//Calculate the end of the trajectory's shooting path
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	FCollisionQueryParams Params;
	//Ignores collision with the Gun itself
	Params.AddIgnoredActor(this);
	//Ignores collision with the owner of the Gun
	Params.AddIgnoredActor(GetOwner());

	//Returns the trajectory of the shooting path
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

//The shooting action
void AGun::PullTrigger() {
	//Adds the animation of the shooting action when the bullet is leaving the gun
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	//Adds the sound of the shooting action when the bullet is leaving the gun
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShootDirection;
	//Calculates if the bullet met an object in its trajectory
	bool Success = GunTrace(Hit, ShootDirection);

	if (Success) {
		//Adds the animation of the shooting action at the collision location
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect , Hit.Location, ShootDirection.Rotation());
		//Adds the sound of the shooting action at the collision location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		//If the object hit was an actor
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr) {
			//Inflict damage to the actor hit
			FPointDamageEvent DamageEvent(Damage, Hit, ShootDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

