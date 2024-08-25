//���õ��˵�����

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"



class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	//��ӵ��˵Ľ�ɫ

	Elementalist, //��ʦ
	Warrior, //սʿ
	Ranger //����

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

	//���˵Ĵ�Ҫ����
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect>SecondaryAttributes;

	//���˵Ļ������ԣ���������������
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect>VitalAttributes;

	//ͨ��ö�ٻ�ȡ��Ӧ�ĳ�ʼ����
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);


};
