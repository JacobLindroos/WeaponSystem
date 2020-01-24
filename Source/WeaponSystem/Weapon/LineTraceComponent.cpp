// Fill out your copyright notice in the Description page of Project Settings.

#include "LineTraceComponent.h"
#include <CollisionQueryParams.h>
#include <Components/ActorComponent.h>
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include <DrawDebugHelpers.h>
#include "WeaponSystem/Weapon/WeaponBase.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include <PhysicalMaterials/PhysicalMaterial.h>


// Sets default values for this component's properties
ULineTraceComponent::ULineTraceComponent()
{
	
}

void ULineTraceComponent::LineTrace(AActor* MyOwner, AWeaponBase* Weapon, float ConeHalfAngleInDegree)
{
	AActor* Player = MyOwner;
	if (Player)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		Player->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (SpreadTrace(EyeRotation.Vector(), ConeHalfAngleInDegree) * 1000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Player);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		//Particle "target" parameter 
		FVector TracerEndPoint = TraceEnd;

		//Blocking hit! Process damage
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, Hit, Player->GetInstigatorController(), Player, DamageType);

			if (Weapon->DefaultImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->DefaultImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			UParticleSystem* SelectedEffect = nullptr;
			switch (SurfaceType)
			{
			case SurfaceType1:
			case SurfaceType2:
				SelectedEffect = Weapon->FleshImpactEffect;
				break;
			default:
				SelectedEffect = Weapon->DefaultImpactEffect;
				break;
			}

			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (bUseLineTrace)
		{
			//drawing a line symbol. the line trace
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
		}

		//adds muzzle effect to weapon
		if (Weapon->MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(Weapon->MuzzleEffect, Weapon->GunMesh, Weapon->MuzzleSocketName);
		}
		
		if (Weapon->TracerEffect)
		{
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->TracerEffect, Weapon->MuzzleLocation);
			if (TracerComp)
			{
				TracerComp->SetVectorParameter(Weapon->TracerTargetName, TracerEndPoint);
			}
		}

		LastFireTime = GetWorld()->TimeSeconds;

	}
}

FVector ULineTraceComponent::SpreadTrace(FVector ConeDir, float ConeHalfAngleInDegree)
{
	return UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ConeDir, ConeHalfAngleInDegree);
}
