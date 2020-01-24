// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/Character.h>
#include "GameFramework/PawnMovementComponent.h"
#include "../Weapon/WeaponBase.h"
#include "WeaponSystem/EnumClasses.h"
#include "Engine/Engine.h"
#include "WeaponSystem/Weapon/AmmoComponent.h"
#include "WeaponSystem/Weapon/PickupInterface.h"
#include "Components/CapsuleComponent.h" 
#include <Components/InputComponent.h>


// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//weapon = CreateDefaultSubobject<AWeaponBase>(TEXT("Weapon"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;



	CurrentlyCarriedBullets = 20;
	MaxBulletsCarried = 99;
	MaxClipCarried = 15;
	CurrentlyCarriedClip = 5;
}


// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	//Attach the gun
	if (CurrentWeapon != nullptr)
	{
		FActorSpawnParameters SpawnGun;
		SpawnGun.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//Spawn the weapon
		weapon = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeapon, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnGun);

		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor - Dont mind the red it funks
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), TEXT("WeaponSocket"));
		weapon->Player = this;

	

	}
}


#pragma region Movement

void AMainPlayer::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMainPlayer::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMainPlayer::BeginCrouch()
{
	Crouch();
}

void AMainPlayer::EndCrouch()
{
	UnCrouch();
}

#pragma endregion


// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement and sight input
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Zoom", this, &AMainPlayer::WeaponZoom);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainPlayer::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainPlayer::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainPlayer::ReloadWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainPlayer::StopAutoFire);
}


FVector AMainPlayer::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}


void AMainPlayer::StartFire()
{
	if (CurrentWeapon != nullptr && weapon != nullptr && weapon->AmmoComp != nullptr)
	{
		if (weapon->FireMode == EFireMode::ESingle)
		{
			weapon->FireSingle();
		}
		if (weapon->FireMode == EFireMode::ESpread)
		{
			weapon->FireSpread();
		}
		if (weapon->FireMode == EFireMode::EBurst)
		{
			weapon->StartBurstTimer();
		}
		if (weapon->FireMode == EFireMode::EAuto)
		{
			weapon->StartAutoFireTimer();
		}
	}

}


void AMainPlayer::StopAutoFire()
{
	if (weapon->FireMode == EFireMode::EAuto)
	{
		weapon->StopAutoFireTimer();
	}
}


void AMainPlayer::ReloadWeapon()
{
	if (weapon != nullptr)
	{
		if (weapon->AmmoType == EAmmoType::EBullets) 
		{
			CurrentlyCarriedBullets = weapon->Reload(CurrentlyCarriedBullets);
		}
		if (weapon->AmmoType == EAmmoType::EClips)
		{
			CurrentlyCarriedClip = weapon->Reload(CurrentlyCarriedClip);
		}
		if (weapon->AmmoType == EAmmoType::EShells)
		{
			CurrentlyCarriedShells = weapon->Reload(CurrentlyCarriedShells);
		}
	}

}



void AMainPlayer::WeaponZoom(float Value)
{
	if (CurrentWeapon != nullptr && weapon != nullptr)
	{
		weapon->Zoom();
	}
}

