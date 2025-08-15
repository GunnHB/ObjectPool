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
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PoolSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxPoolSize = 30;
};