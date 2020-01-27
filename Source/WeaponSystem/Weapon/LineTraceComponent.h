// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTraceComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace", Meta = (ToolTip = "If true linetrace is displayed both from world camera and muzzlelocation of current equipped weapon"))
		bool bUseLineTrace = false;

	UFUNCTION(BlueprintCallable, Category = "LineTrace", Meta = (ToolTip = "Adds a line trace when firing a weapon, depending on what it hit it displays different impact effects. Also add's a muzzle effect."))
		void LineTrace(AActor* MyOwner, AWeaponBase* Weaponfloat, float ConeHalfAngleInDegree = 0.0f);

	//Adds trace effect to spread ammo
	FVector SpreadTrace(FVector ConeDir, float ConeHalfAngleInDegree);

	//Gets the time since last time the weapon is fired, used to calculate auto fire
	float LastFireTime;

protected:
	
};
