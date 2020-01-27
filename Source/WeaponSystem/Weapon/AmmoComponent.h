// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
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

	UFUNCTION(BlueprintPure, Category = "Ammo|Bullets", Meta = (ToolTip = "Get the current amount of bullets in the equipped weapon. If the weapon uses bullets that is, otherwise you can ignore this."))
		int GetBullets() { return CurrentBullets; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Bullets", Meta = (ToolTip = "Get the current amount of bullets in the equipped weapon. If the weapon uses bullets that is, otherwise you can ignore this."))
		int GetMaxBullets() { return MaxBullets; }



	UFUNCTION(BlueprintPure, Category = "Ammo|Clips", Meta = (ToolTip = "Get the current amount of bullets in the clip of the equipped weapon. If the weapon uses clips, otherwise you can ignore this."))
		int GetAmmoInClips() { return CurrentAmmoInClip; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Clips", Meta = (ToolTip = "Get the current amount of bullets in the equipped weapon. If the weapon uses bullets that is, otherwise you can ignore this."))
		int GetMaxAmmoInClips() { return ClipSize; }




	UFUNCTION(BlueprintPure, Category = "Ammo|Shells", Meta = (ToolTip = "Get the current amount of shells in the currently equipped weapon. If the weapon uses shells, otherwise you can ignore this."))
		int GetShells() { return CurrentShells; }

	UFUNCTION(BlueprintPure, Category = "Ammo|Shells", Meta = (ToolTip = "Get the current amount of shells in the currently equipped weapon. If the weapon uses shells, otherwise you can ignore this."))
		int GetMaxShells() { return MaxShells; }


	UFUNCTION(BlueprintPure, Category = "Ammo|Shells", Meta = (ToolTip = "Get how many pellets there is in the shell for this weapon. Only useful if the weapon uses shells. Otherwise ignore this function."))
		int GetPelletsInShell() { return PelletsInShell; }



#pragma endregion


#pragma region Useage of Ammo

	// if it has ammo, use the ammo and return true. if it doesn't, return false
	UFUNCTION(BlueprintCallable, Category = "Ammo|Clips", Meta = (ToolTip = "Just decreses the current ammo in the equipped clip. Only useful if the weapon uses clips, otherwise ignore this fucntion."))
		bool HasAmmo(AWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Ammo|Clips", Meta = (ToolTip = "Just decreses the current ammo in the equipped clip. Only useful if the weapon uses clips, otherwise ignore this fucntion."))
		void DecreseAmmoInClip();

	UFUNCTION(BlueprintCallable, Category = "Ammo|Bullets", Meta = (ToolTip = "Just decreses the current bullets on the weapon. Only useful if the weapon uses bullets, otherwise ignore this fucntion."))
		void DecreseBullets();

	UFUNCTION(BlueprintCallable, Category = "Ammo|Shells", Meta = (ToolTip = "Just decreses the current shells in the weapon. Only useful if the weapon uses shells, otherwise ignore this fucntion."))
		void DecreseShell();

#pragma endregion


#pragma region Reloaders

	UFUNCTION(BlueprintCallable, Category = "Ammo|Clips", Meta = (ToolTip = "Reload the clip in the weapon, only useful if the weapon uses clips."))
		int ReloadClip(int CarriedClips);

	UFUNCTION(BlueprintCallable, Category = "Ammo|Bullets", Meta = (ToolTip = "Reload the bullets in the weapon, only useful if the weapon uses bullets."))
		int ReloadBullets(int CarriedBullets);

	UFUNCTION(BlueprintCallable, Category = "Ammo|Shells", Meta = (ToolTip = "Reload the shells in the weapon, only useful if the weapon uses shells."))
		int ReloadShells(int CarriedShells);

#pragma endregion

public:
	UAmmoComponent();

};
