#pragma once //预处理指令 确保这个头文件只被包含（include）一次，防止重复定义。

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType) //在蓝图中可作为类型使用
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY() //宏 自动生成构造函数、析构函数、拷贝构造函数等

public:


	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return BIsCriticalHit; }

	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(const bool bInIsCriticalHit) { BIsCriticalHit = bInIsCriticalHit; }

	/** 返回用于序列化的实际结构体 */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}


	/** 创建一个副本，用于后续网络复制或者后续修改 */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this; //WithCopy 设置为true，就可以通过赋值操作进行拷贝
		if (GetHitResult())
		{
			// 深拷贝 hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	/** 用于序列化类的参数 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;


protected:

	UPROPERTY()
	bool bIsBlockedHit = false; //格挡

	UPROPERTY()
	bool BIsCriticalHit = false; //暴击
};


template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

