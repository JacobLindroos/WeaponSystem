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
}


void URecoilComponent::Recoil()
{
	if (PController)
	{
		if (--RecoilLoop <= 0)
		{
			StopRecoilTimer();
			StartRecoverTimer();
		}
		PController->AddPitchInput(-1 * RecoilKick);
		PController->AddYawInput(FMath::RandRange(YawMinValue, YawMaxValue));
		PController->ClientPlayCameraShake(CamShake);
	}
}


void URecoilComponent::Recover()
{
	if (--RecoverLoop <= 0)
	{
		StopRecoverTimer();
	}
	PController->AddPitchInput(RecoverKick);
}


void URecoilComponent::StartRecoilTimer()
{
	PController->GetWorldTimerManager().SetTimer(TimerHandle, this, &URecoilComponent::Recoil, InRateTime, true);
}


void URecoilComponent::StopRecoilTimer()
{
	RecoilLoop = intilizeRecoil;
	PController->GetWorldTimerManager().ClearTimer(TimerHandle);
}


void URecoilComponent::StartRecoverTimer()
{
	PController->GetWorldTimerManager().SetTimer(TimerHandle, this, &URecoilComponent::Recover, InRateTime, true);
}


void URecoilComponent::StopRecoverTimer()
{
	RecoverLoop = intilizeRecover;
	PController->GetWorldTimerManager().ClearTimer(TimerHandle);
}
