#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FAuraInputAction
{

	GENERATED_BODY()

	//创建玩家按键类
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	//创建玩家按键的Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

};

UCLASS()
class AURA0_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:


	//查找输入标签函数
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false)const;


	//创建输入的一个容器
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAuraInputAction>AbilityInputAction;




};
