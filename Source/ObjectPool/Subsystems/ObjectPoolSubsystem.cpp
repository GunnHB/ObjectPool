// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"

#include "ObjectPool/DataStruct.h"

UObjectPoolSubsystem::UObjectPoolSubsystem()
{
	ObjectPoolDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Script/Engine.DataTable'/Game/_DataTables/DT_ObjectPool.DT_ObjectPool'"));
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetWorld()->IsGameWorld() == false)
		return;
	
	if (GetWorld()->GetNetMode() == NM_Client)
		return;

	GameStateDelegateHandle = GetWorld()->GameStateSetEvent.AddUObject(this, &ThisClass::BindGameStateSet);
}

void UObjectPoolSubsystem::BindGameStateSet(AGameStateBase* GameState)
{
	if (ObjectPoolDataTable.IsNull() == false)
	{
		UDataTable* LoadedDataTable = ObjectPoolDataTable.LoadSynchronous();
		if (IsValid(LoadedDataTable))
			AsyncLoadObject(LoadedDataTable);
	}
}

void UObjectPoolSubsystem::AsyncLoadObject(UDataTable* DataTable)
{
	TArray<FObjectPoolData*> AllData;
	DataTable->GetAllRows<FObjectPoolData>(__FUNCTION__, AllData);

	for (const FObjectPoolData* Data : AllData)
	{
		// 동기 방식
		UClass* LoadedActor = Data->ActorClass.LoadSynchronous();
		if (IsValid(LoadedActor) == false)
			continue;

		FActorPool& ActorPool = ObjectPoolMap.FindOrAdd(Data->ObjectTag);
		for (int32 Index = 0; Index < Data->PoolSize; ++Index)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AActor* PooledActor = GetWorld()->SpawnActor<AActor>(LoadedActor, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();

	GetWorld()->GameStateSetEvent.Remove(GameStateDelegateHandle);
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

AActor* UObjectPoolSubsystem::GetObjectFromPool(const FGameplayTag InGameplayTag, AActor* Owner)
{
	return nullptr;
}
