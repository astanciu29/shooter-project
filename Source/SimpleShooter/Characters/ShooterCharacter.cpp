// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "SimpleShooter/Actors/Gun.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	SpawnGuns();	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed ,this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ChangeWeapon);
}

void AShooterCharacter::MoveForward(float AxisValue) {
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue) {
	APawn::AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) {  
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue) {
	APawn::AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Jump() {
	ACharacter::Jump();
}

void AShooterCharacter::LookUpRate(float AxisValue) {
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue) {
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot() {
	DefaultGun->PullTrigger();
}

float AShooterCharacter::TakeDamage(float DamageAmount,	struct FDamageEvent const& DamageEvent,	class AController* EventInstigator,	AActor* DamageCauser) {
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//returns lower value
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health is now: %f"), Health);

	if (IsDead()) {
		DetachFromControllerPendingDestroy();
		//Disable the collision with the dead actor
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}
	}
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::SpawnGuns() 
{
	//Hide gun that was already attached to the mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	//Attaching a new gun:
	DefaultGun  = GetWorld()->SpawnActor<AGun>(GunClass);
	DefaultGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	//Setup the character as the owner of the weapon
	DefaultGun->SetOwner(this);
	gunIsLauncher = true;
	//Using 2 Guns, each attached on its own socket. When one is visible, the other is hidden.
	GetMesh()->HideBoneByName(TEXT("WeaponSocket2"), EPhysBodyOp::PBO_None);
	GetMesh()->UnHideBoneByName(TEXT("WeaponSocket"));	
}

void AShooterCharacter::ChangeWeapon()
{
	//Destroy the existing Gun and Spawn another (Launcher/Rifle)
	DefaultGun->Destroy();
	if (gunIsLauncher)
	{
		DefaultGun = GetWorld()->SpawnActor<AGun>(SecondaryGunClass);
		DefaultGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket2"));
		//Setup the character as the owner of the weapon
		DefaultGun->SetOwner(this);
		gunIsLauncher = false;
	}
	else
	{
		DefaultGun = GetWorld()->SpawnActor<AGun>(GunClass);
		DefaultGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		//Setup the character as the owner of the weapon
		DefaultGun->SetOwner(this);
		gunIsLauncher = true;
	}
}

void AShooterCharacter::Heal(float Hp) 
{
	Health = (Hp + Health) > MaxHealth ? MaxHealth : (Hp + Health);
}

void AShooterCharacter::Teleport(FVector Coordinates, FRotator Rotation)
{
	bool teleportationSuccessful = TeleportTo(Coordinates, Rotation, false, false);
	UE_LOG(LogTemp, Warning, TEXT("Teleportation successful - %d"), teleportationSuccessful);
}