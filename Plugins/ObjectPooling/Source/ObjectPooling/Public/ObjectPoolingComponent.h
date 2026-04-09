// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OBJECTPOOLING_API UObjectPoolingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolingComponent();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Configuration */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling")
	bool bInitializeOnBeginPlay = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling")
	TSubclassOf<AActor> ObjectClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling", meta = (ClampMin = 1), meta = (ClampMax = 100))
	int32 PoolSize = 1;

	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	void InitializePool();
	
private:
	UPROPERTY()
	TArray<AActor*> ObjectPool;
	
	void DeinitializePool();
	bool bInitialized = false;
	
	/* Getters and Setters */
public:
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	TArray<AActor*>& GetObjectPool() { return ObjectPool; }
};

