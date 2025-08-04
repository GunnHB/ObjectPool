#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag ObjectTag;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere)
	int32 PoolSize = 0;
};