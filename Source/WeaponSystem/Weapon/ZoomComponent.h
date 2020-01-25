// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZoomComponent.generated.h"

class AMainPlayer;
class APlayerController;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API UZoomComponent : public UActorComponent
{
	GENERATED_BODY()


	AMainPlayer* player;
	APlayerController* PController;


	UPROPERTY(EditAnywhere, Category = "Zoom")
		bool bWantsToZoom = false;

	UPROPERTY(EditAnywhere, Category = "Zoom", Meta = (ToolTip = "Zoomed Field of vision - How much the camera should zoom in."))
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100, ToolTip = "I dont know - gotta check with Jacob"))
		float ZoomInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Zoom", Meta = (ToolTip = "Defualt Field of vision - Where the camera is normally. Set to 0 for no changes."))
		float DefaultFOV;

public:	
	// Sets default values for this component's properties
	UZoomComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Zoom")
		void ZoomHandle();


		
};
