#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FAuraInputAction
{

	GENERATED_BODY()

	//������Ұ�����
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	//������Ұ�����Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

};

UCLASS()
class AURA0_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:


	//���������ǩ����
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false)const;


	//���������һ������
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAuraInputAction>AbilityInputAction;




};
