﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Conversion/SpeckleConverter.h"

#include "BlockConverter.generated.h"

class UBlockInstance;
/**
 * 
 */
UCLASS()
class SPECKLEUNREAL_API UBlockConverter : public UObject, public ISpeckleConverter
{
	GENERATED_BODY()
	
	CONVERTS_SPECKLE_TYPES()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BlockInstanceActorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EComponentMobility::Type> ActorMobility;
	
	UBlockConverter();
	
	virtual AActor* ConvertToNative_Implementation(const UBase* SpeckleBase, ASpeckleUnrealManager* Manager) override;
	virtual UBase* ConvertToSpeckle_Implementation(const UObject* Object, ASpeckleUnrealManager* Manager) override;

	UFUNCTION(BlueprintCallable)
	virtual AActor* BlockToNative(const UBlockInstance* Block, ASpeckleUnrealManager* Manager);
	
protected:
	virtual AActor* CreateEmptyActor(const ASpeckleUnrealManager* Manager, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters = FActorSpawnParameters());
};