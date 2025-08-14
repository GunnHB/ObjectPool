// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ObjectPool/DataStruct.h"
#include "ObjectPool/DebugHelper.h"
#include "ObjectPool/Components/PoolableComponent.h"
#include "ObjectPool/Interfaces/PoolableInterface.h"
#include "ObjectPool/Settings/ObjectPoolSettings.h"

UObjectPoolSubsystem::UObjectPoolSubsystem()
{
	if (GetDefault<UObjectPoolSettings>()->bIsEnabled == false)
		return;
	
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
			
			ActorPool.SpawnActorClass = ActorClass;
			ActorPool.MaxPoolSize = CachedData.MaxPoolSize;
		});

		UAssetManager::GetStreamableManager().RequestAsyncLoad(CachedData.ActorClass.ToSoftObjectPath(), SpawnDelegate);
	}
}

bool UObjectPoolSubsystem::ExpandPool(FActorPool& InActorPool)
{
	if (InActorPool.AllObjects.Num() >= InActorPool.MaxPoolSize)
		return false;

	// 현재 크기만큼 생성하되, 최대 크기는 넘지 않도록
	uint32 NumToSpawn = FMath::Min(InActorPool.AllObjects.Num(), InActorPool.MaxPoolSize - InActorPool.AllObjects.Num());

	if (NumToSpawn <= 0)
		return false;

	for (uint32 Index = 0; Index < NumToSpawn; ++Index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(InActorPool.SpawnActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (IsValid(SpawnedActor) == false)
			continue;

		IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor);
		if (Poolable)
			Poolable->OnPoolDeactivate();

		InActorPool.AllObjects.Emplace(SpawnedActor);
		InActorPool.AvailableObjects.Emplace(SpawnedActor);
	}

	return true;
}

AActor* UObjectPoolSubsystem::GetObjectFromPool(const FGameplayTag& InGameplayTag, AActor* Owner)
{
	FActorPool* ActorPool = ObjectPoolMap.Find(InGameplayTag);
	if (ActorPool == nullptr)
		return nullptr;
	
	if (ActorPool->AvailableObjects.Num() <= 0)
	{
		if (ExpandPool(*ActorPool) == false)
			return nullptr;
	}

	AActor* PooledActor = ActorPool->AvailableObjects.Pop().Get();
	if (IsValid(PooledActor))
	{
		PooledActor->SetOwner(Owner);
		
		IPoolableInterface* Poolable = Cast<IPoolableInterface>(PooledActor);
		if (Poolable != nullptr)
			Poolable->OnPoolActivate(InGameplayTag);
		
		return PooledActor;
	}
	
	return nullptr;
}

void UObjectPoolSubsystem::ReturnObjectToPool(AActor* Actor)
{
	UPoolableComponent* PoolableComponent = Actor->GetComponentByClass<UPoolableComponent>();
	IPoolableInterface* Poolable = Cast<IPoolableInterface>(Actor);

	if (IsValid(PoolableComponent) == false || Poolable == nullptr)
		return;

	FActorPool* ActorPool = ObjectPoolMap.Find(PoolableComponent->GetGameplayTag());
	if (ActorPool == nullptr)
		return;

	ActorPool->AvailableObjects.Emplace(Actor);
	Poolable->OnPoolDeactivate();
}
