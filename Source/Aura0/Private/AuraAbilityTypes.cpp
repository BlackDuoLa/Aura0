
#include "AuraAbilityTypes.h"


//���л���ָ�����ݽṹ�����״̬ת����һ�ֿɴ洢��ɴ���ĸ�ʽ
//�����л������л�������̣��������л�������ݣ�����ļ���������յ��ֽ����У�ת����ԭʼ�Ķ�������ݽṹ��

//
bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;

	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		//�Զ������ݣ����ӱ����͸񵲴����洢�����л�
		//Ŀ���Ƿ����ȡ�����͸�
		//�������͸����л���ת��Ϊ������ʽ������ȡ��ʱ�����л�������ԭ�����͸����ݣ�
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (BIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
	}

	//ʹ���˶��ٳ��ȣ��ͽ���������Ϊ����
	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}


	//�����Ա����񵲵����л������л�����
	//�������͸����л���ת��Ϊ������ʽ������ȡ��ʱ�����л�������ԭ�����͸����ݣ�
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << BIsCriticalHit;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); 
	}

	bOutSuccess = true;
	return true;
}







