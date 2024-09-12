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
//NetMulticast设置后，这个函数被调用时，将在服务器执行，然后复制到每个客户端。
// 和它对应的还有（Server：只在服务器运行，Client：只在调用此函数的客户端运行）这种情况的函数实现需要在后面加上_Implementation
void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);//开启模拟物理效果
	Weapon->SetEnableGravity(true);//开启重力效果
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//开启物理碰撞通道


	GetMesh()->SetSimulatePhysics(true);//开启模拟物理效果
	GetMesh()->SetEnableGravity(true);//开启重力效果
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//开启物理碰撞通道
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);//开启角色与静态物体产生碰撞

	///关闭角色碰撞体通道，避免对武器和角色模拟物理效果产生影响
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

	//申请GE添加属性到自身
	//获取到AbilitySystemComponent
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
	//把添加的GE类型传递过去，并设置初始值为1
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


