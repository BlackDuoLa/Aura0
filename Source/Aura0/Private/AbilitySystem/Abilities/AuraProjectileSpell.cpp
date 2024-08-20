

#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "interaction/CombatInterface.h"





void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 3);


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

		//ȷ���任���ñ���ȷӦ��
		Projectile->FinishSpawning(SpawnTransform);
	}


}
