// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "Aura0\Aura0.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{

	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath();



}
//NetMulticast���ú��������������ʱ�����ڷ�����ִ�У�Ȼ���Ƶ�ÿ���ͻ��ˡ�
// ������Ӧ�Ļ��У�Server��ֻ�ڷ��������У�Client��ֻ�ڵ��ô˺����Ŀͻ������У���������ĺ���ʵ����Ҫ�ں������_Implementation
void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);//����ģ������Ч��
	Weapon->SetEnableGravity(true);//��������Ч��
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//����������ײͨ��


	GetMesh()->SetSimulatePhysics(true);//����ģ������Ч��
	GetMesh()->SetEnableGravity(true);//��������Ч��
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//����������ײͨ��
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);//������ɫ�뾲̬���������ײ

	///�رս�ɫ��ײ��ͨ��������������ͽ�ɫģ������Ч������Ӱ��
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{

	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSockeName);
}




void AAuraCharacterBase::InitAbilityActorInfo()
{

}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{

	//����GE������Ե�����
	//��ȡ��AbilitySystemComponent
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(IsValid(ASC));
	check(GameplayEffectClass);
	//
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);


}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	//����ӵ�GE���ʹ��ݹ�ȥ�������ó�ʼֵΪ1
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);

}

void AAuraCharacterBase::AddCharacterAbilities()
{
	//UAuraAbilitySystemComponentΪʵ����AbilitySystemComponentΪ����ָ��
	//�������˼���ж�AbilitySystemComponent�Ƿ�ΪUAuraAbilitySystemComponent������ָ�룬�ǵĻ���������������
	//��Ϊǰ���Ѿ���UAuraAbilitySystemComponent����������ָ�룬���︳���µ�����ָ��AuraASC����һ��д��������д������ȫ
	//UAuraAbilitySystemComponent* AuraASC =AbilitySystemComponent��(��Ҳ��һ��д��������û������İ�ȫ��
	//��ȫ��: CastChecked ��������ʱ�������ͼ�飬��� AbilitySystemComponent ���������������ͣ�����������ڵ��Խ׶α������������ڴ���������������ٷ��ֺ��޸����⡣
	//�����Ѻ�: ������Ͳ�ƥ�䣬CastChecked �ᴥ�����ԣ��ṩ��ϸ�Ĵ�����Ϣ����������Ժ�׷��������Դ��

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	//HasAuthority() ������ȷ���ڶ���һ����У�ֻ��ӵ��Ȩ������Ϸʵ�������޸��ض���Ϸ�����״̬��
	if (!HasAuthority()) return;

	//�ѻ�ȡ����GA��Ϣ�����ݵ�ASC��
	AuraASC->AddCharacterAbilities(StartupAbilities);



}


