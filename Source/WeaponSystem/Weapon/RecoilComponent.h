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

	int intilizeRecoil = RecoilLoop;
	int intilizeRecover = RecoverLoop;

public:	

	FTimerHandle TimerHandle;
	APlayerController* PController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake", Meta = (ToolTip = ""))
		TSubclassOf<UCameraShake> CamShake;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		float YawMinValue = -0.2;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		float YawMaxValue = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		float RecoilKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		float RecoverKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		int RecoilLoop = 10;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		int RecoverLoop = 5;

	UPROPERTY(EditAnywhere, Category = "Recoil", Meta = (ToolTip = ""))
		float InRateTime = 0.01f;

	// Sets default values for this component's properties
	URecoilComponent();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void StartRecoilTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void StopRecoilTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void StartRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void StopRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void Recoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil", Meta = (ToolTip = ""))
		void Recover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
		
};
