// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileStats", Meta = (ToolTip = "Sets the name of a specific projectile. Use in child BP only."))
		FString name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileStats", Meta = (ToolTip = "Sets what type of projectile it is. Use in child BP only."))
		FString type;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileStats", Meta = (ToolTip = "Which weapon can use this projectile? Use in child BP only."))
		AWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class TSubclassOf<UDamageType> DamageType;

};
