// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RecoilComponent.generated.h"

class AWeaponBase;
class AActor;
class UCameraShake;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API URecoilComponent : public UActorComponent
{
	GENERATED_BODY()
	
	int intilizeRecoil;
	int intilizeRecover;

public:	
	
	FTimerHandle TimerHandle;
	APlayerController* PController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake", Meta = (ToolTip = "Holds the added camera shake"))
		TSubclassOf<UCameraShake> CamShake;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "Min value to add to player´s Yaw/loop"))
		float YawMinValue = -0.2;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "Max value to add to player´s Yaw/loop"))
		float YawMaxValue = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "Sets the value of recoil kick added to pitch, look up input, of the weapon after firing"))
		float RecoilKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "Sets the value of recover kick added to pitch, look up input, of the weapon. Which starts after the recoil kick"))
		float RecoverKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "How many times the recoil function should be executed"))
		int RecoilLoop = 10;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "How many times the recover function should be executed"))
		int RecoverLoop = 5;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = "The amount of time, in seconds, between set and firing."))
		float InRateTime = 0.01f;

	// Sets default values for this component's properties
	URecoilComponent();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Starts the timer of recoil, which calls on the recoil function"))
		void StartRecoilTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Stops the timer of recoil, an calls for the recover function"))
		void StopRecoilTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Starts the timer of recover"))
		void StartRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Stops the timer of recover"))
		void StopRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Handles the recoil after firing weapon"))
		void Recoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = "Handles recover after recoil is done"))
		void Recover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
		
};
