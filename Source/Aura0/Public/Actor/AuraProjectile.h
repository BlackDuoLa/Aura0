
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;



UCLASS()
class AURA0_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraProjectile();


	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;




protected:

	virtual void BeginPlay() override;

	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	//创建有一个球形物体
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;






};
