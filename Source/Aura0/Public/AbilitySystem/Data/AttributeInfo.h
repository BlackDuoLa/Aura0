// ������Ե�����
//ʵ�������ͨ����ͼ�����Ե��ṹ��FAuraAttributeInfo��


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"GameplayTagContainer.h"
#include "AttributeInfo.generated.h"


//�˽ṹ�����ͼ����Ϊ:DA_AttributeInfo
//�����������Ľṹ�壬Info,������͵Ľӿ�����Щ��ʽ
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	//������Ա�ǩ
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	//�����������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	//��û�ж��壬Ӧ���Ǳ�ע����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	//���������ֵ����Ϊ������Ե���ֵ�������ط������ˣ����ﲻ��Ҫ����ڿ�������ڴζ���
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

};



UCLASS()
class AURA0_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()


public:

	//�������ڰ�Tag�ṹ�庯��
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false)const;


	//�������������Ϣ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo>AttributeInformation;


};
