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
#include "../Interact/InteractInterface.h"


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


	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;


	CurrentlyCarriedBullets = 0;
	MaxBulletsCarried = 0;
	MaxClipCarried = 0;
	CurrentlyCarriedClip = 0;

	// declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnOverlapEnd);
}


// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

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
	PlayerInputComponent->BindAction("FireSpecial", IE_Pressed, this, &AMainPlayer::StartFireSpecial);
	PlayerInputComponent->BindAction("FireSpecial", IE_Released, this, &AMainPlayer::StopAutoFireSpecial);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::OnInteract);
	PlayerInputComponent->BindAction("PrimaryWeapon1", IE_Pressed, this, &AMainPlayer::EquipPrimaryWeapon1);
	PlayerInputComponent->BindAction("PrimaryWeapon2", IE_Pressed, this, &AMainPlayer::EquipPrimaryWeapon2);
	PlayerInputComponent->BindAction("SecondaryWeapon1", IE_Pressed, this, &AMainPlayer::EquipSecondaryWeapon1);
	PlayerInputComponent->BindAction("SecondaryWeapon2", IE_Pressed, this, &AMainPlayer::EquipSecondaryWeapon2);

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
	if (CurrentWeapon)
	{
		switch (CurrentWeapon->FireMode)
		{
		case EFireMode::ESingle:
			CurrentWeapon->FireSingle();
			break;
		case EFireMode::ESpread:
			CurrentWeapon->FireSpread();
			break;
		case EFireMode::EBurst:
			CurrentWeapon->StartBurstTimer();
			break;
		case EFireMode::EAuto:
			CurrentWeapon->StartAutoFireTimer();
			break;
		default:
			break;
		}
	}
}

void AMainPlayer::StartFireSpecial()
{
	if (CurrentWeapon)
	{
		switch (CurrentWeapon->FireModeSpecial)
		{
		case EFireMode::ESingle:
			CurrentWeapon->FireSingle(true);
			break;
		case EFireMode::ESpread:
			CurrentWeapon->FireSpread(true);
			break;
		case EFireMode::EBurst:
			CurrentWeapon->StartBurstTimer(true);
			break;
		case EFireMode::EAuto:
			CurrentWeapon->StartAutoFireTimer(true);
			break;
		default:
			break;
		}
	}
}


void AMainPlayer::StopAutoFire()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->CurrentFireMode == EFireMode::EAuto)
		{
			CurrentWeapon->StopAutoFireTimer();
		}
	}
}

void AMainPlayer::StopAutoFireSpecial()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->FireModeSpecial == EFireMode::EAuto)
		{
			CurrentWeapon->StopAutoFireTimer();
		}
	}
}


void AMainPlayer::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		switch (CurrentWeapon->AmmoType)
		{
		case EAmmoType::EBullets:
			CurrentlyCarriedBullets = CurrentWeapon->Reload(CurrentlyCarriedBullets);
			break;
		case EAmmoType::EClips:
			CurrentlyCarriedClip = CurrentWeapon->Reload(CurrentlyCarriedClip);
			break;
		case EAmmoType::EShells:
			CurrentlyCarriedShells = CurrentWeapon->Reload(CurrentlyCarriedShells);
			break;
		default:
			break;
		}
	}
}


void AMainPlayer::WeaponZoom(float Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom();
	}
}

void AMainPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin : %s"), *OtherActor->GetName());

		if (OtherActor != OverlappedActor)
		{
			//if you start overlapping with a new actor while already overlapping with another,
			//force an EndInteract on the first actor you interacted with in favor of the new one.
			if (OverlappedActor)
			{
				IInteractInterface* Interface = Cast<IInteractInterface>(OverlappedActor);
				if (Interface)
				{
					Interface->Execute_OnEndInteract(OverlappedActor);
				}
			}

			//if not overlapping with another actor,
			//either through forcing an EndInteract like above
			//or through a new single overlap,
			//start Interaction with the new actor.
			IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);
			if (Interface)
			{
				Interface->Execute_OnBeginInteract(OtherActor);
			}
			OverlappedActor = OtherActor;
		}

	}
}

void AMainPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap End : %s"), *OtherActor->GetName());

		if (OverlappedActor)
		{
			IInteractInterface* Interface = Cast<IInteractInterface>(OverlappedActor);
			if (Interface)
			{
				Interface->Execute_OnEndInteract(OverlappedActor);
			}
		}
		OverlappedActor = nullptr;
	}
}

void AMainPlayer::OnInteract()
{
	if (OverlappedActor)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(OverlappedActor);
		if (Interface)
		{
			Interface->Execute_OnInteract(OverlappedActor);
			//UE_LOG(LogTemp, Warning, TEXT("OverlappedActor : %s"), *OverlappedActor->GetName());

			if (AWeaponBase* OverlappedWeapon = Cast<AWeaponBase>(OverlappedActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("Picked up weapon : %s"), *OverlappedActor->GetName());
				PickupWeapon(OverlappedWeapon);
			}
		}
	}
}

void AMainPlayer::PickupWeapon(AWeaponBase* Weapon)
{
	switch (Weapon->WeaponCategory)
	{
	case EWeaponCategory::EPrimary:
		if (!PrimaryWeapon1)
		{
			PrimaryWeapon1 = Weapon;
		}
		else if (!PrimaryWeapon2)
		{
			PrimaryWeapon2 = Weapon;
		}
		// if both are occupied, replace first one to new one
		// developer note: this last else needs to be changed into a
		// "drop and replace" system, not currently implemented.
		else 
		{
			PrimaryWeapon1 = Weapon;
		}
		break;
	case EWeaponCategory::ESecondary:
		if (!SecondaryWeapon1)
		{
			SecondaryWeapon1 = Weapon;
		}
		else if (!SecondaryWeapon2)
		{
			SecondaryWeapon2 = Weapon;
		}
		// if both are occupied, replace first one to new one
		// developer note: this last else needs to be changed into a
		// "drop and replace" system, not currently implemented.
		else 
		{
			SecondaryWeapon1 = Weapon;
		}
		break;
	default:
		break;
	}
	Weapon->SetActorHiddenInGame(true);
	Weapon->SetActorEnableCollision(false);
}

void AMainPlayer::EquipPrimaryWeapon1()
{
	EquipWeapon(PrimaryWeapon1);
}

void AMainPlayer::EquipPrimaryWeapon2()
{
	EquipWeapon(PrimaryWeapon2);
}

void AMainPlayer::EquipSecondaryWeapon1()
{
	EquipWeapon(SecondaryWeapon1);
}

void AMainPlayer::EquipSecondaryWeapon2()
{
	EquipWeapon(SecondaryWeapon2);
}

void AMainPlayer::EquipWeapon(AWeaponBase* Weapon)
{
	if (CurrentWeapon)
		CurrentWeapon->SetActorHiddenInGame(true);

	if (!Weapon)
	{
		CurrentWeapon = Weapon;
		return;
	}

	CurrentWeapon = Weapon;
	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
	CurrentWeapon->Player = this;
}
