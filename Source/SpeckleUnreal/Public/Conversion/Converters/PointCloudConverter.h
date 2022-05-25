﻿// Copyright 2022 AEC Systems, Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"
#include "Conversion/SpeckleConverter.h"
#include "Engine/EngineTypes.h"

#include "PointCloudConverter.generated.h"

class ULidarPointCloudComponent;
class ALidarPointCloudActor;
class ULidarPointCloud;
class UPointCloud;

/**
 * Converts Speckle Point Cloud objects into LidarPointClouds
 */
UCLASS()
class SPECKLEUNREAL_API UPointCloudConverter :  public UObject, public ISpeckleConverter
{
	GENERATED_BODY()
	
	CONVERTS_SPECKLE_TYPES()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToNative")
	TSubclassOf<ALidarPointCloudActor> PointCloudActorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToNative")
	TEnumAsByte<EComponentMobility::Type> ActorMobility;
	
	UPointCloudConverter();
	
	virtual UObject* ConvertToNative_Implementation(const UBase* SpeckleBase, UWorld* World, TScriptInterface<ISpeckleConverter>&) override;
	virtual UBase* ConvertToSpeckle_Implementation(const UObject* Object) override;

	UFUNCTION(BlueprintCallable, Category="ToNative")
	virtual ALidarPointCloudActor* PointCloudToNative(const UPointCloud* SpecklePointCloud, UWorld* World);
	
	UFUNCTION(BlueprintCallable, Category="ToSpeckle")
	virtual UPointCloud* PointCloudToSpeckle(const ULidarPointCloudComponent* Object);

protected:
	UFUNCTION(BlueprintCallable, Category="ToNative")
	virtual ALidarPointCloudActor* CreateActor(UWorld* World, ULidarPointCloud* PointCloudData);
};
