
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

	//生成火球函数
	UFUNCTION(BlueprintCallable,Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	//设置生成的物体是什么
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile>ProjectileClass;

	//设置火球伤害
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;
	
};
