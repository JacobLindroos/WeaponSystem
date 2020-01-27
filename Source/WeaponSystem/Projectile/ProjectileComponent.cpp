// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileComponent.h"
#include "../Weapon/WeaponBase.h"
#include "ProjectileBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/Engine.h>

// Sets default values for this component's properties
UProjectileComponent::UProjectileComponent()
{

}


void UProjectileComponent::Fire(float ProjectileSpeed, int NrProjectiles, float ProjectileRange, float MaxProjectileSpreadAngle)
{
	if (!ProjectileClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("ProjectileClass is null in ProjectileComponent."));
		return;
	}

	AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwner());
	FTransform MuzzleTransform = Weapon->GunMesh->GetSocketTransform("MuzzleSocket");
	FVector MuzzleLocation = MuzzleTransform.GetLocation();
	FRotator MuzzleRotation = MuzzleTransform.GetRotation().Rotator();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerCameraManager* CameraManager = World->GetFirstPlayerController()->PlayerCameraManager;

		for (int i = 0; i < NrProjectiles; i++)
		{

			AProjectileBase* Projectile = World->SpawnActorDeferred<AProjectileBase>(ProjectileClass, MuzzleTransform);
			Projectile->SetInitialStats(ProjectileSpeed, ProjectileRange);
			UGameplayStatics::FinishSpawningActor(Projectile, MuzzleTransform);

			FRotator RandomRotation = CameraManager->GetCameraRotation().Add(FMath::FRandRange(-MaxProjectileSpreadAngle, MaxProjectileSpreadAngle), FMath::FRandRange(-MaxProjectileSpreadAngle, MaxProjectileSpreadAngle), FMath::FRandRange(-MaxProjectileSpreadAngle, MaxProjectileSpreadAngle));
			FVector LaunchDirection = RandomRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}
	}
}

// Called when the game starts
void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


