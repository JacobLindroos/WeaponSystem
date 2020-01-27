#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileBase.h"

#include "ProjectileComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProjectileComponent();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void Fire(float ProjectileSpeed, int NrProjectiles, float ProjectileRange, float MaxProjectileSpreadAngle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectileBase> ProjectileClass;
};
