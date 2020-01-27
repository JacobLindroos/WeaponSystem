// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Engine/Engine.h"
#include "AmmoComponent.h"
#include "LineTraceComponent.h"
#include "RecoilComponent.h"
#include "ZoomComponent.h"
#include <GameFramework/Actor.h>
#include "Components/BoxComponent.h" 
#include "WeaponSystem/Projectile/ProjectileComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(CollisionSphereRadius);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("BlockAll"));

	//CollisionComponent->SetupAttachment(RootComponent);
	RootComponent = CollisionComponent;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(CollisionComponent);

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
	RateOfFire = 600;
	MaxImpactMultiplier = 1.0f;

}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	LineComp = FindComponentByClass<ULineTraceComponent>();
	RecoilComp = FindComponentByClass<URecoilComponent>();
	ZoomComp = FindComponentByClass<UZoomComponent>();
	AmmoComp = FindComponentByClass<UAmmoComponent>();
	ProjectileComp = FindComponentByClass<UProjectileComponent>();


	TimeBetweenShots = 60 / RateOfFire;
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MuzzleLocation = GunMesh->GetSocketLocation(MuzzleSocketName);
}


#pragma region Burst timer handler

void AWeaponBase::StartBurstTimer(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if (!bUsingSpecialAttack)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireBurst, 0.08f, true);
	}
	else
	{
		//implement SpecialBurstTimer
		GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireBurst, 0.08f, true);
	}
}

void AWeaponBase::StopBurstTimer()
{
	if (!bUsingSpecialAttack)
	{
		BurstLoop = intilizeBurst;
		GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
	}
	else
	{
		//implement SpecialStopBurstTimer
		BurstLoopSpecial = intilizeBurst;
		GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
	}
}

#pragma endregion


#pragma region Autofire timer handler

void AWeaponBase::StartAutoFireTimer(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if (!bUsingSpecialAttack)
	{
		float FirstDelay = FMath::Max(LineComp->LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

		GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireAuto, TimeBetweenShots, true, FirstDelay);
	}
	else
	{
		//implement SpecialAutoFireTimer
		float FirstDelay = FMath::Max(LineComp->LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

		GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &AWeaponBase::FireAuto, TimeBetweenShots, true, FirstDelay);
	}
}

void AWeaponBase::StopAutoFireTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
}

#pragma endregion


#pragma region Fire functions

void AWeaponBase::FireSingle(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if ((LineComp != nullptr || ProjectileComp != nullptr) && RecoilComp != nullptr)
	{
		if (!bUsingSpecialAttack)
		{
			if (AmmoType == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
			{
				//ProjectileComp->Fire();
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
		else
		{
			//implement SpecialFireSingle
			if (AmmoTypeSpecial == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseBullets();
			}

			if (AmmoTypeSpecial == EAmmoType::EClips)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseAmmoInClip();
			}
		}
	}
}


//Spread Fire, for example Shotguns
void AWeaponBase::FireSpread(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (!bUsingSpecialAttack)
		{
			if (AmmoType == EAmmoType::EShells && AmmoComp->GetShells() > 0)
			{
				for (int i = 0; i < AmmoComp->GetPelletsInShell(); i++)
				{
					LineComp->LineTrace(Player, this, HalfConeDegree);

				}
				AmmoComp->DecreseShell();
				RecoilComp->StartRecoilTimer();
			}
		}
		else
		{
			//implement SpecialFireSpread
			if (AmmoTypeSpecial == EAmmoType::EShells && AmmoComp->GetShells() > 0)
			{
				for (int i = 0; i < AmmoComp->GetPelletsInShell() * 2; i++)
				{
					LineComp->LineTrace(Player, this, HalfConeDegree);

				}
				AmmoComp->DecreseShell();
				RecoilComp->StartRecoilTimer();
			}
		}
	}
}


void AWeaponBase::FireBurst()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (!bUsingSpecialAttack)
		{
			if (--BurstLoop <= 0)
			{
				StopBurstTimer();
			}

			if (AmmoType == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseAmmoInClip();
			}
		}
		else
		{
			//implement SpecialFireBurst
			if (--BurstLoopSpecial <= 0)
			{
				StopBurstTimer();
			}

			if (AmmoTypeSpecial == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseAmmoInClip();
			}
		}
	}
}


void AWeaponBase::FireAuto()
{
	if (LineComp != nullptr && RecoilComp != nullptr)
	{
		if (!bUsingSpecialAttack)
		{

			if (AmmoType == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseBullets();
			}

			if (AmmoType == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseAmmoInClip();
			}
		}
		else
		{
			//implement SpecialFireAuto

			if (AmmoTypeSpecial == EAmmoType::EBullets && AmmoComp->GetBullets() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseBullets();
			}

			if (AmmoTypeSpecial == EAmmoType::EClips && AmmoComp->GetAmmoInClips() > 0)
			{
				LineComp->LineTrace(Player, this);
				RecoilComp->StartRecoilTimer();
				AmmoComp->DecreseAmmoInClip();
			}
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

void AWeaponBase::OnEquipped()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInteract : %s"), *this->GetName())
}

void AWeaponBase::OnBeginInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBeginInteract : %s"), *this->GetName())
}

void AWeaponBase::OnEndInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndInteract : %s"), *this->GetName())
}




