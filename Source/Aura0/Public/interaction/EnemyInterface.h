// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA0_API IEnemyInterface
{
	GENERATED_BODY()


public:
	//添加接口需要实现的函数
	//沟边
	virtual  void HightLightActor() = 0;
	//不沟边
	virtual  void UnHightLightActor() = 0;


};
