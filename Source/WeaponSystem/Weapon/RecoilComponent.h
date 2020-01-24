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

public:	
	// Sets default values for this component's properties
	URecoilComponent();

	FTimerHandle TimerHandle;
	APlayerController* PController;

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void StartRecoilTimer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
		TSubclassOf<UCameraShake> CamShake;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		float YawMinValue = -0.2;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		float YawMaxValue = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		float RecoilKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		float RecoverKick = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		int RecoilLoop = 10;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		int RecoverLoop = 5;

	UPROPERTY(EditAnywhere, Category = "Recoil")
		float InRateTime = 0.01f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void StopRecoilTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void StartRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void StopRecoverTimer();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void Recoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
		void Recover();

private:

	int intilizeRecoil = RecoilLoop;
	int intilizeRecover = RecoverLoop;


		
};
