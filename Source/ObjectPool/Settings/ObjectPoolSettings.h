// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ObjectPoolSettings.generated.h"

/**
 * 
 */
UCLASS(config = ObjectPoolSetting, Defaultconfig, meta = (DisplayName = "Object Pool Setting"))
class OBJECTPOOL_API UObjectPoolSettings : public UDeveloperSettings
{
	GENERATED_BODY()

protected:
	UPROPERTY(Config, EditAnywhere)
	bool bIsEnabled = true;
};
