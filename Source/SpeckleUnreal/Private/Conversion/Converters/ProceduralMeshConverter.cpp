﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Conversion/Converters/ProceduralMeshConverter.h"

#include "ProceduralMeshComponent.h"
#include "Conversion/Converters/MaterialConverter.h"
#include "Materials/MaterialInstance.h"
#include "Objects/DisplayValueElement.h"
#include "Objects/Geometry/Mesh.h"
#include "Objects/Other/RenderMaterial.h"
#include "Objects/Utils/SpeckleObjectUtils.h"

UProceduralMeshConverter::UProceduralMeshConverter()
{
    SpeckleTypes.Add(UMesh::StaticClass());
    SpeckleTypes.Add(UDisplayValueElement::StaticClass());
    
    MeshActorType = AActor::StaticClass();
    ActorMobility = EComponentMobility::Static;
}

UObject* UProceduralMeshConverter::ConvertToNative_Implementation(const UBase* SpeckleBase, UWorld* World, TScriptInterface<ISpeckleConverter>& AvailableConverters )
{
    const UMesh* m = Cast<UMesh>(SpeckleBase);
    if(m != nullptr)	
        return MeshToNative(m, World, AvailableConverters);
    
    const UDisplayValueElement* d = Cast<UDisplayValueElement>(SpeckleBase);
    if(d)
    {
        AActor* Parent = CreateEmptyActor(World, FTransform());
        for(const UMesh* Child : d->DisplayValue)
        {
            AActor* ChildActor = MeshToNative(Child, World, AvailableConverters);
            if(IsValid(ChildActor))
            {
#if WITH_EDITOR
                ChildActor->SetActorLabel(FString::Printf(TEXT("%s - %s"), *Child->SpeckleType, *Child->Id));
#endif
                ChildActor->GetRootComponent()->SetMobility(ActorMobility);
                ChildActor->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
                ChildActor->SetOwner(Parent);
            }
        }
        return Parent;
    }
    return nullptr;
}

AActor* UProceduralMeshConverter::MeshToNative(const UMesh* SpeckleMesh, UWorld* World, TScriptInterface<ISpeckleConverter>& MaterialConverter)
{
    AActor* MeshActor = CreateEmptyActor(World, USpeckleObjectUtils::CreateTransform(SpeckleMesh->Transform));
    UProceduralMeshComponent* MeshComponent = NewObject<UProceduralMeshComponent>(MeshActor, FName("SpeckleMeshComponent"));
    MeshComponent->SetupAttachment(MeshActor->GetRootComponent());
    MeshComponent->RegisterComponent();
    
    TArray<int32> Faces;

    int32 i = 0;
    while (i < SpeckleMesh->Faces.Num())
    {
        int32 n = SpeckleMesh->Faces[i];
        if(n < 3) n += 3; // 0 -> 3, 1 -> 4
        
        if (n == 3) //Triangles
        {
            Faces.Add(SpeckleMesh->Faces[i + 3]);
            Faces.Add(SpeckleMesh->Faces[i + 2]);
            Faces.Add(SpeckleMesh->Faces[i + 1]);
        }
        else if(n == 4) // Quads
        {
            Faces.Add(SpeckleMesh->Faces[i + 4]);
            Faces.Add(SpeckleMesh->Faces[i + 3]);
            Faces.Add(SpeckleMesh->Faces[i + 1]);

            Faces.Add(SpeckleMesh->Faces[i + 3]);
            Faces.Add(SpeckleMesh->Faces[i + 2]);
            Faces.Add(SpeckleMesh->Faces[i + 1]);
        }
        else
        {
            // n-gons shall be ignored
        }
        
        i += n + 1;
    }
    
    const TArray<FVector> Normals;
    const TArray<FProcMeshTangent> Tangents;
    
    MeshComponent->CreateMeshSection(
        0,
        SpeckleMesh->Vertices,
        Faces,
        Normals,
        SpeckleMesh->TextureCoordinates,
        SpeckleMesh->VertexColors,
        Tangents,
        true);

    UMaterialInterface* Material = Cast<UMaterialInstance>(Execute_ConvertToNative(MaterialConverter.GetObject(), SpeckleMesh->RenderMaterial, World, MaterialConverter));
    ensure(Material != nullptr);
    
    MeshComponent->SetMaterial(0, Material);
    
    return MeshActor;
}

AActor* UProceduralMeshConverter::CreateEmptyActor(UWorld* World, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters)
{
    AActor* Actor = World->SpawnActor<AActor>(MeshActorType, Transform, SpawnParameters);
    USceneComponent* Scene = NewObject<USceneComponent>(Actor, "Root");
    Actor->SetRootComponent(Scene);
    Scene->RegisterComponent();
    Scene->SetMobility(ActorMobility);
    return Actor;
}


UBase* UProceduralMeshConverter::ConvertToSpeckle_Implementation(const UObject* Object)
{
    const UProceduralMeshComponent* M = Cast<UProceduralMeshComponent>(Object);

    if(M == nullptr)
    {
        const AActor* A = Cast<AActor>(Object);
        if(A != nullptr)
        {
            M = A->FindComponentByClass<UProceduralMeshComponent>();
        }
    }
    if(M == nullptr) return nullptr;
	
    return MeshToSpeckle(M);
}


UMesh* UProceduralMeshConverter::MeshToSpeckle(const UProceduralMeshComponent* Object)
{
    return nullptr; //TODO implement ToSpeckle function
}
