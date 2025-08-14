// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OBJECTPOOL_API IPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPoolActivate(const FGameplayTag& InGameplayTag) = 0;
	virtual void OnPoolDeactivate() = 0;
};
