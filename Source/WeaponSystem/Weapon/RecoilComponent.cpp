// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilComponent.h"
#include "WeaponBase.h"
#include <GameFramework/PlayerController.h>
#include <Components/ActorComponent.h>
#include "TimerManager.h"
#include "RecoilComponent.h"
#include <Engine/Engine.h>
#include "Camera/PlayerCameraManager.h"
#include "../Player/MainPlayer.h"

// Sets default values for this component's properties
URecoilComponent::URecoilComponent()
{
	//Setting default values 
	YawMaxValue = 0.5f;
	RecoilKick = 0.5f;
	RecoverKick = 0.5f;
	RecoilLoop = 10;
	RecoverLoop = 5;
	InRateTime = 0.01f;
	YawMinValue = -0.2f;
}


// Called when the game starts
void URecoilComponent::BeginPlay()
{
	Super::BeginPlay();
	PController = GetWorld()->GetFirstPlayerController();

	intilizeRecoil = RecoilLoop;
	intilizeRecover = RecoverLoop;
}


//Adds a recoil kick to the weapon
void URecoilComponent::Recoil()
{
	if (PController)
	{
		if (--RecoilLoop <= 0)
		{
			//Stops the timer of recoil and start recover
			StopRecoilTimer();
			StartRecoverTimer();
		}
		//Adds the specified recoil kick/loop, a random Yaw recoil and camera shake of the weapon
		PController->AddPitchInput(-1 * RecoilKick);
		PController->AddYawInput(FMath::RandRange(YawMinValue, YawMaxValue));
		PController->ClientPlayCameraShake(CamShake);
	}
}


//Adds a recover kick after the recoil kick
void URecoilComponent::Recover()
{
	if (--RecoverLoop <= 0)
	{
		//Stops the recover kick
		StopRecoverTimer();
	}
	//Adds the recover kick/loop
	PController->AddPitchInput(RecoverKick);
}


//Starts the timer that handles the recoil
void URecoilComponent::StartRecoilTimer()
{
	PController->GetWorldTimerManager().SetTimer(TimerHandle, this, &URecoilComponent::Recoil, InRateTime, true);
}


//Stops the timer that handles the recoil and resets the loop value for next time
void URecoilComponent::StopRecoilTimer()
{
	RecoilLoop = intilizeRecoil;
	PController->GetWorldTimerManager().ClearTimer(TimerHandle);
}


//Starts the timer that handles the recover
void URecoilComponent::StartRecoverTimer()
{
	PController->GetWorldTimerManager().SetTimer(TimerHandle, this, &URecoilComponent::Recover, InRateTime, true);
}


//Stops the timer that handles the recover and resets the loop value for next time
void URecoilComponent::StopRecoverTimer()
{
	RecoverLoop = intilizeRecover;
	PController->GetWorldTimerManager().ClearTimer(TimerHandle);
}
