// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/Engine.h>
#include "../Player/MainPlayer.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(5.0f);
	// Set the root component to be the collision component.
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

}

void AProjectileBase::SetInitialStats(float ProjectileSpeed, float PjtRange)
{
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	ProjectileRange = PjtRange;

	InitialLifeSpan = PjtRange / ProjectileSpeed;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

float AProjectileBase::CalculateDamageMultiplier(float Distance)
{
	DRQuotient = Distance / ProjectileRange;

	// if target is further away than the base damage distance.
	if (DRQuotient >= 0.5 && DRQuotient <= 1)
	{
		ImpactMultiplier = (DRQuotient - 0.5) / 0.5;
		DamageMultiplier = (1 - ImpactMultiplier);
	}
	// if target is closer than the base damage distance.
	else if (DRQuotient < 0.5 && DRQuotient > 0)
	{
		ImpactMultiplier = abs(DRQuotient - 1);
		DamageMultiplier = (1 + ImpactMultiplier) * (Weapon->MaxImpactMultiplier / 2);
	}
	return DamageMultiplier;
}

float AProjectileBase::CalculateOutputDamage(float Distance, float BaseDamage)
{
	DamageMultiplier = CalculateDamageMultiplier(Distance);

	OutputDamage = BaseDamage * DamageMultiplier;
	return OutputDamage;
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Hit: ") + OtherActor->GetName());

		AMainPlayer* Player = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

		Weapon = Cast<AWeaponBase>(Player->CurrentWeapon);

		if (Player)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			Player->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();

			FVector PlayerLocation = Player->GetActorLocation();

			float Distance = FVector::Dist(PlayerLocation, OtherActor->GetActorLocation());
			OutputDamage = CalculateOutputDamage(Distance, Weapon->BaseDamage);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("OutputDamage: ") + FString::SanitizeFloat(OutputDamage));

			UGameplayStatics::ApplyPointDamage(OtherActor, OutputDamage, ShotDirection, Hit, Player->GetInstigatorController(), Player, TSubclassOf<UDamageType>());
		}
	}
}

