﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Base.h"
#include "UObject/Interface.h"

#include "SpeckleConverter.generated.h"

class UBase;
class ASpeckleUnrealManager;
class ISpeckleConverter;

UINTERFACE()
class SPECKLEUNREAL_API USpeckleConverter : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for conversion functions (ToSpeckle and ToNative) of a specific speckle type(s).
 *
 *  Implementors of this interface provide conversion functions of specific types : Base
 */
class SPECKLEUNREAL_API ISpeckleConverter
{
	GENERATED_BODY()

public:
	/// Will return true if this converter can convert a given BaseType
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanConvertToNative(TSubclassOf<UBase> BaseType);
	
	/// Tries to convert a given SpeckleBase into a native Actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UObject* ConvertToNative(const UBase* SpeckleBase, UWorld* World, UPARAM(ref) TScriptInterface<ISpeckleConverter>& AvailableConverters);

	/// Clean up cached resources, and finish any pending build tasks to complete ToNative conversion.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishConversion();

	
	/// Will return true if this converter can convert a given 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CanConvertToSpeckle(const AActor* Actor);
	
	/// Tries to convert a given Actor or Component into a Speckle Base
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ConvertToSpeckle(const UObject* Object, UBase* SpeckleObject);



	
	
};

#define CONVERTS_SPECKLE_TYPES() \
protected: \
	UPROPERTY(EditAnywhere, BlueprintReadWrite) \
	TSet<TSubclassOf<UBase>> SpeckleTypes; \
public: \
	virtual bool CanConvertToNative_Implementation(TSubclassOf<UBase> BaseType) override { return SpeckleTypes.Contains(BaseType); } \
private: