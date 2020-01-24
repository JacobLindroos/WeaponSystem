// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/Engine.h"
#include "WeaponBase.h"
#include "AmmoComponent.h"
#include "LineTraceComponent.h"
#include "RecoilComponent.h"
#include "ZoomComponent.h"
#include <GameFramework/Actor.h>
#include "Components/BoxComponent.h" 


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
	RateOfFire = 600;
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	LineComp = FindComponentByClass<ULineTraceComponent>();
	RecoilComp = FindComponentByClass<URecoilComponent>();
	ZoomComp = FindComponentByClass<UZoomComponent>();
	AmmoComp = FindComponentByClass<UAmmoComponent>();

	TimeBetweenShots = 60 / RateOfFire;
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MuzzleLocation = GunMesh->GetSocketLocation(MuzzleSocketName);
}


#pragma region Burst timer handler

void AWeaponBase::StartBurstTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireBurst, 0.08f, true);
}

void AWeaponBase::StopBurstTimer()
{
	BurstLoop = intilizeBurst;
	GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
}

#pragma endregion


#pragma region Autofire timer handler

void AWeaponBase::StartAutoFireTimer()
{
	float FirstDelay = FMath::Max(LineComp->LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

	GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireAuto, TimeBetweenShots, true, FirstDelay);
}

void AWeaponBase::StopAutoFireTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
}

#pragma endregion


#pragma region Fire functions

void AWeaponBase::FireSingle()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{

		if (AmmoType == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Ammo: %d"), AmmoComp->GetBullets()));
			LineComp->LineTrace(Player, this);
			RecoilComp->StartRecoilTimer();
			AmmoComp->DecreseBullets();
		}

		if (AmmoType == EAmmoType::EClips)
		{
			LineComp->LineTrace(Player, this);
			RecoilComp->StartRecoilTimer();
			AmmoComp->DecreseAmmoInClip();
		}

	}
}


//Spread Fire, for example Shotguns
void AWeaponBase::FireSpread()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (AmmoType == EAmmoType::EShells && AmmoComp->GetShells() > 0)
		{
			for ( int i = 0; i < AmmoComp->GetPelletsInShell(); i++)
			{
				LineComp->LineTrace(Player, this, HalfConeDegree);

			}
			AmmoComp->DecreseShell();
			RecoilComp->StartRecoilTimer();
		}
		
	}
}


void AWeaponBase::FireBurst()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (--BurstLoop <= 0)
		{
			StopBurstTimer();
		}

		if (AmmoType == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Ammo: %d"), AmmoComp->GetAmmoInClips()));
			LineComp->LineTrace(Player, this);
			RecoilComp->StartRecoilTimer();
			AmmoComp->DecreseAmmoInClip();
		}
	}
}


void AWeaponBase::FireAuto()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (AmmoType == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Ammo: %d"), AmmoComp->GetBullets()));
			LineComp->LineTrace(Player, this);
			RecoilComp->StartRecoilTimer();
			AmmoComp->DecreseBullets();
		}

		if (AmmoType == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Ammo: %d"), AmmoComp->GetAmmoInClips()));
			LineComp->LineTrace(Player, this);
			RecoilComp->StartRecoilTimer();
			AmmoComp->DecreseAmmoInClip();
		}
	}
}

#pragma endregion


void AWeaponBase::Zoom()
{
	if (ZoomComp != nullptr)
	{
		ZoomComp->ZoomHandle();
	}
}

int AWeaponBase::Reload(int CurrentHeldAmmo)
{
	if (AmmoComp != nullptr)
	{
		if (AmmoType == EAmmoType::EBullets)
		{
			CurrentHeldAmmo = AmmoComp->ReloadBullets(CurrentHeldAmmo);
		}

		if (AmmoType == EAmmoType::EClips)
		{
			CurrentHeldAmmo = AmmoComp->ReloadClip(CurrentHeldAmmo);
		}

		if (AmmoType == EAmmoType::EShells)
		{
			CurrentHeldAmmo = AmmoComp->ReloadShells(CurrentHeldAmmo);
		}
	}


	return CurrentHeldAmmo;
}




