// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile Collider"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	bIsSelected = false;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PopFX, GetActorLocation(), FRotator::ZeroRotator)->SetColorParameter(FxParamName, FxColor);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::ChangeTileSelected()
{
	bIsSelected = !bIsSelected;
	if(bIsSelected)
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Emissive_Pow"), 100.f);
		}	
	}
	else
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Emissive_Pow"), 0.f);
		}
	}
}
