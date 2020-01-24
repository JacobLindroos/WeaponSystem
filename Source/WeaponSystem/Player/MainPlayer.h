// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.generated.h"

class AWeaponBase;

UCLASS()
class WEAPONSYSTEM_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int MaxBulletsCarried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int CurrentlyCarriedBullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int MaxClipCarried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int CurrentlyCarriedClip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int MaxShells;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoStats")
		int CurrentlyCarriedShells;

	// create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;

	UPROPERTY()
		AActor* OverlappedActor;

public:

	UPROPERTY(VisibleAnywhere)
		AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere)
		AWeaponBase* PrimaryWeapon1;

	UPROPERTY(VisibleAnywhere)
		AWeaponBase* PrimaryWeapon2;

	UPROPERTY(VisibleAnywhere)
		AWeaponBase* SecondaryWeapon1;

	UPROPERTY(VisibleAnywhere)
		AWeaponBase* SecondaryWeapon2;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UCameraComponent* GetCamera() { return CameraComp; }

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StopAutoFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void ReloadWeapon();

	UFUNCTION()
		void WeaponZoom(float Value);

	// pick up and equip
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Interact")
		void OnInteract();

	UFUNCTION()
		void PickupWeapon(AWeaponBase* Weapon);

	UFUNCTION()
		void EquipPrimaryWeapon1();
	UFUNCTION()
		void EquipPrimaryWeapon2();
	UFUNCTION()
		void EquipSecondaryWeapon1();
	UFUNCTION()
		void EquipSecondaryWeapon2();
	UFUNCTION()
		void EquipWeapon(AWeaponBase* Weapon);

};
