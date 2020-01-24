// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

	//The max amount ammo the clip can have.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Clips")
		int ClipSize;

	//How much ammo there's in the clip.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Clips")
		int CurrentAmmoInClip;

	//The max amount of bullets the gun can have.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Bullets")
		int MaxBullets;

	//How many bullets the gun currently has.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Bullets")
		int CurrentBullets;

	//The max amount of shells the gun can have.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Shells")
		int MaxShells;

	//How many shells the gun currently has.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Shells")
		int CurrentShells;

	//How many pellets the shell contains, used for spread shot.
	UPROPERTY(EditAnywhere, Category = "AmmoStats|Shells")
		int PelletsInShell;

public:

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "Ammo|Bullets")
		int GetBullets() { return CurrentBullets; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Clips")
		int GetAmmoInClips() { return CurrentAmmoInClip; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Shells")
		int GetShells() { return CurrentShells; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Shells")
		int GetPelletsInShell() { return PelletsInShell; }

#pragma endregion


#pragma region Useage of Ammo

	UFUNCTION(BlueprintCallable, Category = "Ammo|Clips")
		void DecreseAmmoInClip();

	UFUNCTION(BlueprintCallable, Category = "Ammo|Bullets")
		void DecreseBullets();

	UFUNCTION(BlueprintCallable, Category = "Ammo|Shells")
		void DecreseShell();

#pragma endregion


#pragma region Reloaders

	UFUNCTION(BlueprintCallable, Category = "Ammo|Clips")
		int ReloadClip(int CarriedClips);

	UFUNCTION(BlueprintCallable, Category = "Ammo|Bullets")
		int ReloadBullets(int CarriedBullets);

	UFUNCTION(BlueprintCallable, Category = "Ammo|Shells")
		int ReloadShells(int CarriedShells);

#pragma endregion

public:
	UAmmoComponent();

};
