// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<class AWeaponBase> CurrentWeapon;


	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int MaxBulletsCarried;

	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int CurrentlyCarriedBullets;

	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int MaxClipCarried;

	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int CurrentlyCarriedClip;

	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int MaxShells;

	UPROPERTY(EditAnywhere, Category = "AmmoStats")
		int CurrentlyCarriedShells;
	
	UPROPERTY()
		AWeaponBase* weapon;

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
};
