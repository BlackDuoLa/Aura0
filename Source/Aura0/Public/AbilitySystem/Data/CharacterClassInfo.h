//设置敌人的属性

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"



class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	//添加敌人的角色

	Elementalist, //法师
	Warrior, //战士
	Ranger //游侠

};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()



	UPROPERTY(EditDefaultsOnly,Category = "Class Defaults")
	TSubclassOf<UGameplayEffect>PrimaryAttributes;


};






UCLASS()
class AURA0_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:


	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo>CharacterClassInformation;

	//敌人的次要属性
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect>SecondaryAttributes;

	//敌人的基础属性（生命力、法力）
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect>VitalAttributes;

	//通过枚举获取对应的初始化类
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);


};
