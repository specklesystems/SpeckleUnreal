﻿// Copyright 2022 AEC Systems, Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Conversion/SpeckleConverter.h"

#include "AggregateConverter.generated.h"

/**
 *  An Aggregate Converter stores multiple ISpeckleConverter instances.
 *  This allows you to use many converters as one
 */
UCLASS()

class SPECKLEUNREAL_API UAggregateConverter : public UObject, public ISpeckleConverter
{
	GENERATED_BODY()

protected:
	
	// A lazily initialised mapping of SpeckleType -> converters.
	TMap<TSubclassOf<UBase>, TScriptInterface<ISpeckleConverter>> SpeckleTypeMap;

public:
	
	// Array of converters, must be of type ISpeckleConverter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speckle|Conversion")
	TArray<UObject*> SpeckleConverters;
	
	UFUNCTION(BlueprintCallable, Category="Speckle|Conversion")
	virtual UObject* ConvertToNativeInternal(const UBase* Object, UWorld* World);
	virtual UObject* ConvertToNative_Implementation(const UBase* SpeckleBase, UWorld* World, TScriptInterface<ISpeckleConverter>&) override;

	virtual bool CanConvertToNative_Implementation(TSubclassOf<UBase> BaseType) override;

	
	virtual void ConvertToSpeckle_Implementation(const UObject* Object, UBase* SpeckleObject) override;

	
	UFUNCTION(BlueprintCallable, Category="Speckle|Conversion")
	virtual TScriptInterface<ISpeckleConverter> GetConverter(const TSubclassOf<UBase> BaseType);

	// Validates a given object is a valid ISpeckleConverter
	static bool CheckValidConverter(const UObject* Converter, bool LogWarning = true);

	void FinishConversion_Internal();
	virtual void FinishConversion_Implementation() override;

	// Validates changes to SpeckleConverters property, Should be called after modifying SpeckleConverters
	UFUNCTION(BlueprintCallable, Category="Speckle|Conversion")
	virtual void OnConvertersChangeHandler();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
