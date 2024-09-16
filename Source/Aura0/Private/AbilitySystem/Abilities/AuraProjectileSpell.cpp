

#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "interaction/CombatInterface.h"
#include "Aura0/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	


}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	//获取当前玩家控制的窗口是否为服务器窗口
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	//if (!bIsServer)return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		//获取到武器位置，并设置武器的旋转
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		//设置武器位置
		SpawnTransform.SetLocation(SocketLocation);
		//设置武器旋转
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//SpawnActorDeferred将异步创建实例，在实例创建完成时，相应的数据已经应用到了实例身上
		//生成火球

	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		const UAbilitySystemComponent*SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());


		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);//设置技能
		EffectContextHandle.AddSourceObject(Projectile);//设置GE的源
		//添加Actor列表
		TArray<TWeakObjectPtr<AActor>>Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		//添加命中结果
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		//添加技能触发位置
		EffectContextHandle.AddOrigin(ProjectileTargetLocation);

		const  FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());



		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		//玩家等级和伤害进行同步，玩家多少级对等到伤害多少级（伤害为多少）
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 20);


		Projectile->DamageEffectSpecHandle = SpecHandle;

		//确保变换设置被正确应用
		Projectile->FinishSpawning(SpawnTransform);
	}


}
