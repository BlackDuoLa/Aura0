// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(IsValid(ASC));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);


}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	//
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


