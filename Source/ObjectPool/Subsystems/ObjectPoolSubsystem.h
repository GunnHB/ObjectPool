// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

/**
 * 
 */

USTRUCT()
struct FActorPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Pool;
};

UCLASS()
class OBJECTPOOL_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UObjectPoolSubsystem();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	AActor* GetObjectFromPool(const FGameplayTag InGameplayTag, AActor* Owner);

protected:
	UPROPERTY()
	TSoftObjectPtr<UDataTable> ObjectPoolDataTable = nullptr;

	UPROPERTY()
	TMap<FGameplayTag, FActorPool> ObjectPoolMap;

private:
	void InitPool();
	void AsyncLoadObject(UDataTable* DataTable);
};
