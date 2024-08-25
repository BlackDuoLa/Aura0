// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Aura0GameModeBase.generated.h"


class UCharacterClassInfo;


UCLASS()
class AURA0_API AAura0GameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	//创建敌人属性的接口
	UPROPERTY(EditDefaultsOnly,Category = "Character Class Dufaults")
	TObjectPtr<UCharacterClassInfo>CharacterClassInfo;










	
};
