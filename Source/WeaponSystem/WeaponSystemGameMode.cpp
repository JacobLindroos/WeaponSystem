// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WeaponSystemGameMode.h"
#include "WeaponSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWeaponSystemGameMode::AWeaponSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
