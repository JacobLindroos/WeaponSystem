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
#include <Kismet/GameplayStatics.h>
#include <Math/TransformNonVectorized.h>
#include <Kismet/KismetMathLibrary.h>

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

void AWeaponBase::SpawnSingleProjectile()
{
	if (!ProjectileComp->ProjectileClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::FString("ProjectileComp->ProjectileClass is null"));
		return;
	}

	FTransform MuzzleTransform = GunMesh->GetSocketTransform(MuzzleSocketName);
	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileComp->ProjectileClass, MuzzleTransform);
	Projectile->SetInitialStats(ProjectileComp->ProjectileSpeed, ProjectileComp->ProjectileRange);
	UGameplayStatics::FinishSpawningActor(Projectile, MuzzleTransform);
	Projectile->FireInDirection(MuzzleTransform.GetRotation().Rotator().Vector());
}

void AWeaponBase::SpawnMultipleProjectiles(float SpreadDegree, int NrSpawnProjectiles)
{
	if (!ProjectileComp->ProjectileClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::FString("ProjectileComp->ProjectileClass is null")); 
		return;
	}

	FTransform MuzzleTransform = GunMesh->GetSocketTransform(MuzzleSocketName);
	for (int i = 0; i < NrSpawnProjectiles; i++)
	{
		AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileComp->ProjectileClass, MuzzleTransform);
		Projectile->SetInitialStats(ProjectileComp->ProjectileSpeed, ProjectileComp->ProjectileRange);
		UGameplayStatics::FinishSpawningActor(Projectile, MuzzleTransform);

		FVector RandomVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(MuzzleTransform.GetRotation().Rotator().Vector(), SpreadDegree);
		Projectile->FireInDirection(RandomVector);
	}
}

#pragma endregion


#pragma region Fire functions

void AWeaponBase::FireSingle(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if (!LineComp || !ProjectileComp ||  !RecoilComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("One or more of following components is(are) missing : LineComp, ProjectileComp, RecoilComp"));
		return;
	}

	// Change FireMode and AmmoType depends on bUsingSpecialAttack value.
	if (bUsingSpecialAttack)
	{
		CurrentFireMode = FireModeSpecial;
		CurrentAmmoType = AmmoTypeSpecial;
	}
	else
	{
		CurrentFireMode = FireMode;
		CurrentAmmoType = AmmoType;
	}

	// Check if the weapon has ammo. if so, use a ammo and return true. otherwise return false
	if (!AmmoComp->HasAmmo(this))
	{
		return;
	}

	// Check FiringSystem, CurrentFireMode, bUsingSpecialAttack and then fire the weapon accordingly.
	Fire();
}

//Spread Fire, for example Shotguns
void AWeaponBase::FireSpread(bool bSpecialAttack)
{
	bUsingSpecialAttack = bSpecialAttack;
	if (!LineComp || !ProjectileComp || !RecoilComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("One or more of following components is(are) missing : LineComp, ProjectileComp, RecoilComp"));
		return;
	}

	// Change FireMode and AmmoType depends on bUsingSpecialAttack value.
	if (bUsingSpecialAttack)
	{
		CurrentFireMode = FireModeSpecial;
		CurrentAmmoType = AmmoTypeSpecial;
	}
	else
	{
		CurrentFireMode = FireMode;
		CurrentAmmoType = AmmoType;
	}

	// Check if the weapon has ammo. if so, use a ammo and return true. otherwise return false
	if (!AmmoComp->HasAmmo(this))
	{
		return;
	}

	// Check FiringSystem, CurrentFireMode, bUsingSpecialAttack and then fire the weapon accordingly.
	Fire();
}

void AWeaponBase::FireBurst()
{
	if (!LineComp || !ProjectileComp || !RecoilComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("One or more of following components is(are) missing : LineComp, ProjectileComp, RecoilComp"));
		return;
	}

	// Change FireMode and AmmoType depends on bUsingSpecialAttack value.
	if (bUsingSpecialAttack)
	{
		CurrentFireMode = FireModeSpecial;
		CurrentAmmoType = AmmoTypeSpecial;
	}
	else
	{
		CurrentFireMode = FireMode;
		CurrentAmmoType = AmmoType;
	}

	// Check if the weapon has ammo. if so, use a ammo and return true. otherwise return false
	if (!AmmoComp->HasAmmo(this))
	{
		StopBurstTimer();
		return;
	}

	// Check FiringSystem, CurrentFireMode, bUsingSpecialAttack and then fire the weapon accordingly.
	Fire();
}

void AWeaponBase::FireAuto()
{
	if (!LineComp || !ProjectileComp || !RecoilComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("One or more of following components is(are) missing : LineComp, ProjectileComp, RecoilComp"));
		return;
	}

	// Change FireMode and AmmoType depends on bUsingSpecialAttack value.
	if (bUsingSpecialAttack)
	{
		CurrentFireMode = FireModeSpecial;
		CurrentAmmoType = AmmoTypeSpecial;
	}
	else
	{
		CurrentFireMode = FireMode;
		CurrentAmmoType = AmmoType;
	}

	// Check if the weapon has ammo. if so, use a ammo and return true. otherwise return false
	if (!AmmoComp->HasAmmo(this))
	{
		StopBurstTimer();
		return;
	}

	// Check FiringSystem, CurrentFireMode, bUsingSpecialAttack and then fire the weapon accordingly.
	Fire();
}

void AWeaponBase::Fire()
{
	switch (FiringSystem)
	{
	case EFiringSystem::ELineTrace:
		switch (CurrentFireMode)
		{
		case EFireMode::EAuto:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> EAuto -> Special"));
				LineComp->LineTrace(Player, this);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> EAuto -> Primary"));
				LineComp->LineTrace(Player, this);
			}
			break;
		case EFireMode::EBurst:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> EBurst -> Special"));
				LineComp->LineTrace(Player, this);
				if (--BurstLoopSpecial <= 0)
				{
					StopBurstTimer();
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> EBurst -> Primary"));
				LineComp->LineTrace(Player, this);
				if (--BurstLoop <= 0)
				{
					StopBurstTimer();
				}
			}
			break;
		case EFireMode::ESingle:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> ESingle -> Special"));
				LineComp->LineTrace(Player, this);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> ESingle -> Primary"));
				LineComp->LineTrace(Player, this);
			}
			break;
		case EFireMode::ESpread:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> ESpread -> Special"));
				for (int i = 0; i < AmmoComp->GetPelletsInShell(); i++)
				{
					LineComp->LineTrace(Player, this, HalfConeDegree);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ELineTrace -> ESpread -> Primary"));
				for (int i = 0; i < AmmoComp->GetPelletsInShell(); i++)
				{
					LineComp->LineTrace(Player, this, HalfConeDegree);
				}
			}
			break;
		default:
			break;
		}
		break;
	case EFiringSystem::EProjectile:
		switch (CurrentFireMode)
		{
		case EFireMode::EAuto:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> EAuto -> Special"));
				SpawnSingleProjectile();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> EAuto -> Primary"));
				SpawnSingleProjectile();
			}
			break;
		case EFireMode::EBurst:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> EBurst -> Special"));
				SpawnSingleProjectile();
				if (--BurstLoopSpecial <= 0)
				{
					StopBurstTimer();
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> EBurst -> Primary"));
				SpawnSingleProjectile();
				if (--BurstLoop <= 0)
				{
					StopBurstTimer();
				}
			}
			break;
		case EFireMode::ESingle:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> ESingle -> Special"));
				SpawnSingleProjectile();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> ESingle -> Primary"));
				SpawnSingleProjectile();
			}
			break;
		case EFireMode::ESpread:
			if (bUsingSpecialAttack)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> ESpread -> Special"));
				SpawnMultipleProjectiles(HalfConeDegree, AmmoComp->GetPelletsInShell());
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("EProjectile -> ESpread -> Primary"));
				SpawnMultipleProjectiles(HalfConeDegree, AmmoComp->GetPelletsInShell());
			}
			break;
		default:
			break;
		}
	default:
		break;
	}

	RecoilComp->StartRecoilTimer();
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




