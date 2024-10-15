// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	SlideDuration = .8f;
	bIsSelected = false;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	int32 randNum = UKismetMathLibrary::RandomIntegerInRange(0, (int32)ETileType::MaxTileType - 1);
	TileType = (ETileType)randNum;
	
	if(TileMesh[(int32)TileType])
	{
		MeshComp->SetStaticMesh(TileMesh[(int32)TileType]);
	}
	if(TileMaterial[(int32)TileType])
	{
		MeshComp->SetMaterial(0, TileMaterial[(int32)TileType]);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsSliding)
	{
		if(CurrentTime <= SlideDuration)
		{
			CurrentTime += DeltaTime;
			SetActorLocation(FMath::Lerp(CurrentLoc, ToMoveLoc, CurrentTime / SlideDuration));
		}
		else
		{
			bIsSliding = false;
			CurrentTime = 0.0f;
		}
	}
}

void ATile::SetTileSelected()
{
	bIsSelected = !bIsSelected;
	if(bIsSelected)
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Select_Str"), 5.f);
		}	
	}
	else
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Select_Str"), 0.f);
		}
	}
}

void ATile::SetCoord(FIntPoint NewCoord)
{
	CoordX = NewCoord.X;
	CoordY = NewCoord.Y;
}

FIntPoint ATile::GetCoord()
{
	return FIntPoint(CoordX, CoordY);
}

void ATile::SetTileLocation(FIntPoint NewCoord)
{
	CurrentLoc = GetActorLocation();
	ToMoveLoc = GetActorLocation() + FVector((NewCoord.X - CoordX) * 100.f, (NewCoord.Y - CoordY) * 100.f, 0.0f);
	CoordX = NewCoord.X;
	CoordY = NewCoord.Y;
	bIsSliding = true;
}

