// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJECTPOOL_API UPoolableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPoolableComponent();
	
	void OnActivate();
	void OnDeactivate();

	bool GetIsActivate() const { return bIsActivate; }

private:
	bool bIsActivate = false;
};
