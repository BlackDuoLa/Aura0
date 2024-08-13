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

	//申请GE添加属性到自身
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
	//UAuraAbilitySystemComponent为实例，AbilitySystemComponent为智能指针
	//这里的意思是判断AbilitySystemComponent是否为UAuraAbilitySystemComponent的智能指针，是的话就允许这样操作
	//因为前面已经对UAuraAbilitySystemComponent赋予了智能指针，这里赋予新的智能指针AuraASC，的一种写法。这种写法更安全
	//UAuraAbilitySystemComponent* AuraASC =AbilitySystemComponent；(这也是一种写法，但是没有上面的安全。
	//安全性: CastChecked 会在运行时进行类型检查，如果 AbilitySystemComponent 不是你期望的类型，代码会立刻在调试阶段崩溃。这种早期错误检测有助于你快速发现和修复问题。
	//调试友好: 如果类型不匹配，CastChecked 会触发断言，提供详细的错误信息，帮助你调试和追踪问题来源。

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	//HasAuthority() 是用于确保在多玩家环境中，只有拥有权威的游戏实例才能修改特定游戏对象的状态。
	if (!HasAuthority()) return;

	//把获取到的GA消息，传递到ASC中
	AuraASC->AddCharacterAbilities(StartupAbilities);



}


