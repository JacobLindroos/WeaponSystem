#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumClasses.generated.h"

UENUM()
enum class EWeaponCategory : uint8
{
	EPrimary		UMETA(DisplayName = "Primary Weapon", ToolTip = "This weapon is a primary weapon."),
	ESecondary		UMETA(DisplayName = "Secondary Weapon", ToolTip = "This weapon is a secondary weapon.")
};

UENUM()
enum class EFireMode : uint8
{
	ESingle		UMETA(DisplayName = "Single", ToolTip = "Shoots a single bullet."),
	EBurst		UMETA(DisplayName = "Burst", ToolTip = "Shoot multiple bullets at a time."),
	ESpread		UMETA(DisplayName = "Spread", ToolTip = "Shoots a few bullets at a time."),
	EAuto		UMETA(DisplayName = "Auto", ToolTip = "Shoots bullets one after another in a fast pace.")
};


UENUM()
enum class EWeaponType : uint8
{
	EPistol				UMETA(DisplayName = "Pistol", ToolTip = "Modest rate of fire, limited range, useful in close proximity, but useless in most other cases."),
	ECompactGun			UMETA(DisplayName = "CompactGuns", ToolTip = "High rate of fire, modest range, extremely versatile weapon."),
	EAssultRifle		UMETA(DisplayName = "AssultRifles", ToolTip = "High rate of fire, modest range, a very versatile weapon."),
	EShotgun			UMETA(DisplayName = "Shotgun", ToolTip = "Low rate of fire, limited range, but excellent damage!"),
	ESniperRifle		UMETA(DisplayName = "SniperRifle", ToolTip = "Low rate of fire, long range, great at picking off targets, useless in most other cases.")
};


UENUM()
enum class EAmmoType : uint8
{
	EBullets		UMETA(DisplayName = "Bullets", ToolTip = "Just loose bullets, often used for guns like sniper rifles and revolvers etc."),
	EClips			UMETA(DisplayName = "Clips", ToolTip = "Clips have a set amount of bullets in them, mostly used in rifles etc."),
	EShells			UMETA(DisplayName = "Shells", ToolTip = "Shells such as shotgun shells, contains a set amount of 'pellets'. Used in the Spread-shot mode.")

};