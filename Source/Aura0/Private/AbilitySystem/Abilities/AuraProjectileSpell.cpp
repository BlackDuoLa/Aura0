

#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "interaction/CombatInterface.h"





void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 3);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer)return;
	//生成Actor

	


	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if(CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();




		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		//SpawnActorDeferred将异步创建实例，在实例创建完成时，相应的数据已经应用到了实例身上
		//生成火球
		AAuraProjectile*Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);



		//确保变换设置被正确应用
		Projectile->FinishSpawning(SpawnTransform);


	} 











		
}
