
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"


class AAuraProjectile;

UCLASS()
class AURA0_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()


public:


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle ,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;



	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile>ProjectileClass;


	
};
