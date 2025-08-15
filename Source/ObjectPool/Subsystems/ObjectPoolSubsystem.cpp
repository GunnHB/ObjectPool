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

	LoadDataTable();
}

void UObjectPoolSubsystem::LoadDataTable()
{
	if (ObjectPoolDataTable.IsNull())
		return;

	FStreamableDelegate InitPoolDelegate = FStreamableDelegate::CreateLambda([this]
	{
		UDataTable* LoadedDataTable = ObjectPoolDataTable.Get();
		if (IsValid(LoadedDataTable))
			InitPool(LoadedDataTable);
	});

	UAssetManager::GetStreamableManager().RequestAsyncLoad(ObjectPoolDataTable.ToSoftObjectPath(), InitPoolDelegate);
}

void UObjectPoolSubsystem::InitPool(const UDataTable* InDataTable)
{
	TArray<FObjectPoolData*> AllData;
	InDataTable->GetAllRows<FObjectPoolData>(__FUNCTION__, AllData);

	for (const FObjectPoolData* Data : AllData)
	{
		if (Data->ObjectTag == FGameplayTag::EmptyTag || IsValid(Data->ActorClass) == false)
			continue;

		FActorPool& ActorPool = ObjectPoolMap.FindOrAdd(Data->ObjectTag);
		for (int32 Index = 0; Index < Data->PoolSize; ++Index)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Data->ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (IsValid(SpawnedActor) == false)
				continue;
	
			IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor);
			if (Poolable)
				Poolable->OnPoolDeactivate();
	
			ActorPool.AllObjects.Emplace(SpawnedActor);
			ActorPool.AvailableObjects.Emplace(SpawnedActor);
		}

		ActorPool.SpawnActorClass = Data->ActorClass;
		ActorPool.MaxPoolSize = Data->MaxPoolSize;
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
