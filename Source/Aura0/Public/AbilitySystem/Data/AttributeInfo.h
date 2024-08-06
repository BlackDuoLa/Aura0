// 玩家属性的输入
//实现了玩家通过蓝图绑定属性到结构体FAuraAttributeInfo中


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"GameplayTagContainer.h"
#include "AttributeInfo.generated.h"


//此结构体的蓝图名字为:DA_AttributeInfo
//玩家属性输出的结构体，Info,输出类型的接口有哪些形式
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	//玩家属性标签
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	//玩家属性名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	//还没有定义，应该是备注属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	//玩家属性数值
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

};



UCLASS()
class AURA0_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()


public:

	//创建绑定结构体函数
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound)const;


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo>AttributeInformation;


};
