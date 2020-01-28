// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include <Kismet/GameplayStatics.h>

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

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::SetInitialStats(float ProjectileSpeed, float PjtRange)
{
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != this && OtherComponent->ComponentHasTag("Enemy"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Hit: ") + OtherActor->GetName());

		AMainPlayer* Player = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

		Weapon = Cast<AWeaponBase>(Player->CurrentWeapon);

		UGameplayStatics::ApplyPointDamage(OtherActor, 20.f, this->GetActorForwardVector(), Hit, Player->GetInstigatorController(), Player, DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->FleshImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

		Destroy();
	}
}