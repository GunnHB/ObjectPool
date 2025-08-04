// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ObjectPool/DataStruct.h"
#include "ObjectPool/Components/PoolableComponent.h"
#include "ObjectPool/Interfaces/PoolableInterface.h"

UObjectPoolSubsystem::UObjectPoolSubsystem()
{
	ObjectPoolDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/_DataTables/DT_ObjectPool.DT_ObjectPool'")));
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (GetWorld()->GetNetMode() == NM_Client || ObjectPoolMap.Num() > 0)
		return;

	InitPool();
}

void UObjectPoolSubsystem::InitPool()
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
		const FObjectPoolData CachedData = *Data;

		FStreamableDelegate SpawnDelegate = FStreamableDelegate::CreateLambda([this, CachedData]()
		{
			UClass* ActorClass = CachedData.ActorClass.Get();
			if (IsValid(ActorClass) == false)
				return;

			FActorPool& ActorPool = ObjectPoolMap.FindOrAdd(CachedData.ObjectTag);
			for (int32 Index = 0; Index < CachedData.PoolSize; Index++)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				if (IsValid(SpawnedActor) == false)
					continue;

				IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor);
				if (Poolable)
					Poolable->OnPoolDeactivate();

				ActorPool.AllObjects.Emplace(SpawnedActor);
				ActorPool.AvailableObjects.Emplace(SpawnedActor);
			}
		});

		UAssetManager::GetStreamableManager().RequestAsyncLoad(CachedData.ActorClass.ToSoftObjectPath(), SpawnDelegate);
	}
}

AActor* UObjectPoolSubsystem::GetObjectFromPool(const FGameplayTag InGameplayTag, AActor* Owner)
{
	FActorPool* ActorPool = ObjectPoolMap.Find(InGameplayTag);
	if (ActorPool == nullptr)
		return nullptr;

	// 동적으로 크기 늘리는 로직 추가
	if (ActorPool->AvailableObjects.Num() == 0)
		return nullptr;

	AActor* PooledActor = ActorPool->AvailableObjects.Pop().Get();
	if (IsValid(PooledActor))
	{
		PooledActor->SetOwner(Owner);
		return PooledActor;
	}
	
	return nullptr;
}
