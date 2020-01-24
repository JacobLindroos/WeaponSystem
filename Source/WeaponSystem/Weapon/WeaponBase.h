// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSystem/EnumClasses.h"
#include "WeaponSystem/Player/MainPlayer.h"
#include <Components/PrimitiveComponent.h>
#include "WeaponBase.generated.h"

class ULineTraceComponent;
class UCameraComponent;
class URecoilComponent;
class UZoomComponent;
class UAmmoComponent;

UCLASS()
class WEAPONSYSTEM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:

	//The collision comp
	UPROPERTY(VisibleAnywhere, Category = "Collider")
		class UBoxComponent* CollisionComp;

	//The sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class USoundBase* FireSound;

public:
	FTimerHandle TimeHandler;
	UAmmoComponent* AmmoComp;
	ULineTraceComponent* LineComp;
	URecoilComponent* RecoilComp;
	UZoomComponent* ZoomComp;

	//The mesh of the gun
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USkeletalMeshComponent* GunMesh;

	// Sets default values for this actor's properties
	AWeaponBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AMainPlayer* Player;

	//The name of the gun. Nothing complicated here.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunStats")
		FName GunName;

	// The time in seconds it takes to reload the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunStats|Reload")
		float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats")
		TEnumAsByte<EFireMode> FireMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats")
		TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats")
		TEnumAsByte<EAmmoType> AmmoType;

	UPROPERTY(EditAnywhere, Category = "GunStats|Recoil")
		int BurstLoop = 3;

	//RPM - Bullets per minute fired by weapon
	UPROPERTY(EditDefaultsOnly, Category = "GunStats", Meta = (ClampMin = 0, ClampMax = 100))
		float RateOfFire;

	//Derived from RateOfFire
	float TimeBetweenShots;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FVector MuzzleLocation;




	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunStats")
		float HalfConeDegree = 10.f;


protected:

	virtual void BeginPlay() override;

	int intilizeBurst = BurstLoop;



public:

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Recoil|Burst")
		void StartBurstTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil|Burst")
		void StopBurstTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil|Auto")
		void StartAutoFireTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil|Auto")
		void StopAutoFireTimer();



	UFUNCTION()
		virtual void FireSingle();

	UFUNCTION()
		virtual void FireSpread();

	UFUNCTION()
		virtual void FireBurst();

	UFUNCTION()
		virtual void FireAuto();

	UFUNCTION(Category = "Weapon|Zoom")
		void Zoom();

	UFUNCTION(Category = "Weapon|Reload")
		int Reload(int CurrentHeldAmmo);


};
