// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoComponent.h"
#include <Engine/Engine.h>
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	ClipSize = 0;
	CurrentAmmoInClip = 0;

	MaxBullets = 0;
	CurrentBullets = 0;

	MaxShells = 0;
	CurrentShells = 0;
	PelletsInShell = 0;

}



bool UAmmoComponent::HasAmmo(AWeaponBase* Weapon)
{
	// ammo use - if there is no ammo left, return(no fire)
	switch (Weapon->CurrentAmmoType)
	{
	case EAmmoType::EClips:
		if (CurrentAmmoInClip > 0)
		{
			CurrentAmmoInClip--;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Out of Ammo."));
			return false;
		}
		break;
	case EAmmoType::EBullets:
		if (CurrentBullets > 0)
		{
			CurrentBullets--;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Out of Ammo."));
			return false;
		}
		break;
	case EAmmoType::EShells:
		if (CurrentShells > 0)
		{
			CurrentShells--;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Out of Ammo."));
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

void UAmmoComponent::DecreseAmmoInClip()
{
	//Reduces ammo in clip
	if (CurrentAmmoInClip > 0)
	{
		CurrentAmmoInClip--;
	}
}

void UAmmoComponent::DecreseBullets()
{
	//Reduces bullets in weapon
	if (CurrentBullets > 0)
	{
		CurrentBullets--;
	}
}

void UAmmoComponent::DecreseShell()
{
	if (CurrentShells > 0)
	{
		CurrentShells--;
	}
}


int UAmmoComponent::ReloadClip(int CarriedClips)
{

	if (CarriedClips > 0)
	{
		CurrentAmmoInClip = ClipSize;
		CarriedClips--;
	}

	return CarriedClips;
}


int UAmmoComponent::ReloadBullets(int CarriedBullets)
{
	if (CarriedBullets > 0)
	{
		int TempBullets = CurrentBullets;

		CurrentBullets = UKismetMathLibrary::Clamp(CarriedBullets, 0, MaxBullets);

		CarriedBullets -= (CurrentBullets - TempBullets);
	}

	return CarriedBullets;
}

int UAmmoComponent::ReloadShells(int CarriedShells)
{
	if (CarriedShells > 0)
	{
		int TempShells = CurrentShells;

		CurrentShells = UKismetMathLibrary::Clamp(CarriedShells, 0, MaxShells);

		CarriedShells -= (CurrentShells - TempShells);
	}

	return CarriedShells;
}


