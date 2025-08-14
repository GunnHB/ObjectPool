// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.generated.h"


class UObjectPoolSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJECTPOOL_API UPoolableComponent : public UActorComponent
{
	GENERATED_BODY()

	friend UObjectPoolSubsystem;

public:
	UPoolableComponent();
	
	void OnActivate(const FGameplayTag& InGameplayTag);
	void OnDeactivate();

	FGameplayTag GetGameplayTag() const { return Tag; }
	bool GetIsActivate() const { return bIsActivate; }

private:
	FGameplayTag Tag;
	
	bool bIsActivate = false;
};
