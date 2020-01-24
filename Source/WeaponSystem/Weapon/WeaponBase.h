// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSystem/EnumClasses.h"
#include "WeaponSystem/Player/MainPlayer.h"
#include <Components/PrimitiveComponent.h>
#include <Components/SphereComponent.h>
#include "../Interact/InteractInterface.h"
#include "WeaponBase.generated.h"

class ULineTraceComponent;
class UCameraComponent;
class URecoilComponent;
class UZoomComponent;
class UAmmoComponent;

UCLASS()
class WEAPONSYSTEM_API AWeaponBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Collider")
		class UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", Meta = (ToolTip = "When firing the gun, if a sound is desired, add it here."))
		class USoundBase* FireSound;

public:

	//Just some pointers, do not touch please.

	FTimerHandle TimeHandler;
	UAmmoComponent* AmmoComp;
	ULineTraceComponent* LineComp;
	URecoilComponent* RecoilComp;
	UZoomComponent* ZoomComp;




	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta = (ToolTip = "The mesh of the gun, nothing more fun than that."))
		class USkeletalMeshComponent* GunMesh;


	UPROPERTY()
		AMainPlayer* Player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunStats", Meta = (ToolTip = "The name of this gun, not the same as weapon type."))
		FName GunName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats", Meta = (ToolTip = "What type of gun it is."))
		TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats", Meta = (ToolTip = "Is the weapon a Primary Weapon or a Secondary Weapon."))
		TEnumAsByte<EWeaponCategory> WeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats", Meta = (ToolTip = "Which type of mode the gun is supposed to have."))
		TEnumAsByte<EFireMode> FireMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gunstats", Meta = (ToolTip = "What type of ammo it has."))
		TEnumAsByte<EAmmoType> AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunStats|Reload", Meta = (ToolTip = "The time it takes in seconds to reload the weapon."))
		float ReloadTime;


	UPROPERTY(EditAnywhere, Category = "GunStats", Meta = (ToolTip = "How many bullets that should be shot at a time. Not the same as a the spread! This is for the burst."))
		int BurstLoop = 3;


	UPROPERTY(EditDefaultsOnly, Category = "GunStats", Meta = (ClampMin = 0, ToolTip = "RPM - Bullets per minute, cannot be less than 0."))
		float RateOfFire;

	//Derived from RateOfFire
	float TimeBetweenShots;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunStats", Meta = (ToolTip = "The hit location is calculated using a cone, if 0 it will hit straight forward, if something else it will hit randomly within that radius."))
		float HalfConeDegree;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FVector MuzzleLocation;



#pragma region Cool Effects and shit

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", Meta = (ToolTip = "If the gun should have an effect at the muzzle while shooting."))
		class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", Meta = (ToolTip = "If the gun should have some effect when hitting something."))
		class UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", Meta = (ToolTip = "When the gun hits something, such as flesh, then add the desired effect here. "))
		class UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", Meta = (ToolTip = "When traveling through the air, if it should have some kind of 'tail' effect, then it should be added here."))
		class UParticleSystem* TracerEffect;

#pragma endregion



	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		bool bOverlapping;

	// Sets default values for this actor's properties
	AWeaponBase();

protected:

	virtual void BeginPlay() override;

	int intilizeBurst = BurstLoop;



public:

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Recoil|Burst", Meta = (ToolTip = "Start tje nurt fire functuion."))
		void StartBurstTimer();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Recoil|Burst", Meta = (ToolTip = ""))
		void StopBurstTimer();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Recoil|Auto", Meta = (ToolTip = ""))
		void StartAutoFireTimer();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Recoil|Auto", Meta = (ToolTip = ""))
		void StopAutoFireTimer();



	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire", Meta = (ToolTip = "Basic bithc shots. I've lsot focus I'm sorryu."))
		virtual void FireSingle();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire", Meta = (ToolTip = "Spread fire, like shotguns and stuff. Spawn several bullets that do bullet stuff. Overwrite may be ok."))
		virtual void FireSpread();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire", Meta = (ToolTip = "Burst fire, shoot idk like 3 bullets at a time. You have my permission to fuucking overwrite it if you need to."))
		virtual void FireBurst();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire", Meta = (ToolTip = "Automatic fire, already implemented in code, overwrite it tho if you want."))
		virtual void FireAuto();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Zoom", Meta = (ToolTip = "The basic zoom function, needs a 'Zoom' component to work."))
		void Zoom();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Add-on|Reload", Meta = (ToolTip = "The reload function. Implemented in code, but if you want something else then just overwrite it with blueprint."))
		int Reload(int CurrentHeldAmmo);

	UFUNCTION(BlueprintCallable)
		void OnEquipped();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract();
		virtual void OnInteract_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnBeginInteract();
		virtual void OnBeginInteract_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnEndInteract();
		virtual void OnEndInteract_Implementation();

};
