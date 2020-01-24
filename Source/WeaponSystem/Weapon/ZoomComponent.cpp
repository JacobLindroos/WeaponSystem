// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoomComponent.h"
#include "../Player/MainPlayer.h"
#include "Camera/CameraComponent.h"
#include <Components/ActorComponent.h>
#include <GameFramework/PlayerController.h>


// Sets default values for this component's properties
UZoomComponent::UZoomComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20;
}


// Called when the game starts
void UZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	PController = GetWorld()->GetFirstPlayerController();
	player = Cast<AMainPlayer>(PController->GetPawn());

	DefaultFOV = player->GetCamera()->FieldOfView;
}


// Called every frame
void UZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (player)
	{
		float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

		float NewFOV = FMath::FInterpTo(player->GetCamera()->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

		player->GetCamera()->SetFieldOfView(NewFOV);
	}
}


void UZoomComponent::ZoomHandle()
{
	if (PController->WasInputKeyJustPressed(EKeys::RightMouseButton))
	{
		//player->GetCamera()->SetFieldOfView(65.f);
		bWantsToZoom = true;
	}
	else if(PController->WasInputKeyJustReleased(EKeys::RightMouseButton))
	{
		//player->GetCamera()->SetFieldOfView(90.f);
		bWantsToZoom = false;
	}
}

