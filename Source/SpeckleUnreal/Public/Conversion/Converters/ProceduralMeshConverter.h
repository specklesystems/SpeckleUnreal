﻿// Copyright 2022 AEC Systems, Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"
#include "Conversion/SpeckleConverter.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"

#include "ProceduralMeshConverter.generated.h"

class UMaterialConverter;
class UProceduralMeshComponent;
class UMesh;
class URenderMaterial;


/**
 * Converts Speckle Mesh objects into native actors with a procedural mesh component.
 * 
 * Compared with the StaticMeshConverter, this converter has some serious limitations
 * - Cannot convert displayValues,
 * - N-gon faces will be ignored,
 * - Meshes are transient, and won't persist on level reload
 */
UCLASS()
class SPECKLEUNREAL_API UProceduralMeshConverter :  public UObject, public ISpeckleConverter
{
	GENERATED_BODY()

	CONVERTS_SPECKLE_TYPES()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToNative")
	TSubclassOf<AActor> MeshActorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToNative")
	TEnumAsByte<EComponentMobility::Type> ActorMobility;
	
	// Sets default values for this actor's properties
	UProceduralMeshConverter();
	
	virtual UObject* ConvertToNative_Implementation(const UBase* SpeckleBase, UWorld* World, TScriptInterface<ISpeckleConverter>& AvailableConverters) override;
	virtual UBase* ConvertToSpeckle_Implementation(const UObject* Object) override;
	
	UFUNCTION(BlueprintCallable, Category="ToNative")
	virtual AActor* MeshToNative(const UMesh* SpeckleMesh, UWorld* World, TScriptInterface<ISpeckleConverter>& MaterialConverter);
	
	UFUNCTION(BlueprintCallable, Category="ToNative")
	virtual UMesh* MeshToSpeckle(const UProceduralMeshComponent* Object);
	
	virtual AActor* CreateEmptyActor(UWorld* World, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters = FActorSpawnParameters());
	
};
