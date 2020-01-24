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

	UPROPERTY(EditAnywhere, Category = "Zoom")
		bool bWantsToZoom = false;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float DefaultFOV;

	AMainPlayer* player;
	APlayerController* PController;
		
};
