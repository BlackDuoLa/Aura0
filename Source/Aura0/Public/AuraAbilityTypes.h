#pragma once //Ԥ����ָ�� ȷ�����ͷ�ļ�ֻ��������include��һ�Σ���ֹ�ظ����塣

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType) //����ͼ�п���Ϊ����ʹ��
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY() //�� �Զ����ɹ��캯���������������������캯����

public:


	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return BIsCriticalHit; }

	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(const bool bInIsCriticalHit) { BIsCriticalHit = bInIsCriticalHit; }

	/** �����������л���ʵ�ʽṹ�� */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}


	/** ����һ�����������ں������縴�ƻ��ߺ����޸� */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this; //WithCopy ����Ϊtrue���Ϳ���ͨ����ֵ�������п���
		if (GetHitResult())
		{
			// ��� hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	/** �������л���Ĳ��� */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;


protected:

	UPROPERTY()
	bool bIsBlockedHit = false; //��

	UPROPERTY()
	bool BIsCriticalHit = false; //����
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

