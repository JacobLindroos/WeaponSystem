#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "../Weapon/WeaponBase.h"

#include "ProjectileBase.generated.h"

UCLASS()
class WEAPONSYSTEM_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void SetInitialStats(float ProjectileSpeed, float ProjectileRange);

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileStats")
		FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileStats")
		FString type;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileStats")
		AWeaponBase* Weapon;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileStats")
		float ProjectileRange;

private:
	float ImpactMultiplier;
	float DamageMultiplier;
	float DRQuotient;
	float OutputDamage;

	float CalculateDamageMultiplier(float Distance);
	float CalculateOutputDamage(float Distance, float BaseDamage);
};
