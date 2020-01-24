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

	UFUNCTION(BlueprintCallable, Category = "LineTrace")
		void LineTrace(AActor* MyOwner, AWeaponBase* Weaponfloat, float ConeHalfAngleInDegree = 0.0f);

	FVector SpreadTrace(FVector ConeDir, float ConeHalfAngleInDegree);

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace")
		bool bUseLineTrace = false;

	float LastFireTime;

protected:
	
};
