

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
	//��ȡ��ǰ��ҿ��ƵĴ����Ƿ�Ϊ����������
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	//if (!bIsServer)return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		//��ȡ������λ�ã���������������ת
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		//��������λ��
		SpawnTransform.SetLocation(SocketLocation);
		//����������ת
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//SpawnActorDeferred���첽����ʵ������ʵ���������ʱ����Ӧ�������Ѿ�Ӧ�õ���ʵ������
		//���ɻ���

	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		const UAbilitySystemComponent*SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());


		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);//���ü���
		EffectContextHandle.AddSourceObject(Projectile);//����GE��Դ
		//���Actor�б�
		TArray<TWeakObjectPtr<AActor>>Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		//������н��
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		//��Ӽ��ܴ���λ��
		EffectContextHandle.AddOrigin(ProjectileTargetLocation);

		const  FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());



		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		//��ҵȼ����˺�����ͬ������Ҷ��ټ��Եȵ��˺����ټ����˺�Ϊ���٣�
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 20);


		Projectile->DamageEffectSpecHandle = SpecHandle;

		//ȷ���任���ñ���ȷӦ��
		Projectile->FinishSpawning(SpawnTransform);
	}


}
