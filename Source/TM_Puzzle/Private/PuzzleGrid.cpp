// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGrid.h"
#include "Tile.h"

// Sets default values
APuzzleGrid::APuzzleGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APuzzleGrid::BeginPlay()
{
	Super::BeginPlay();
	
	TileGrid.SetNum(GridHeight * GridWidth);
	
	for (int32 i = 0; i < GridHeight; i++)
	{
		for (int32 j = 0; j < GridWidth; j++)
		{
			if(TileClass)
			{
				SpawnTileToGrid(FIntPoint(i, j));
			}
		}
	}
	// for (int32 i = 0; i < GridWidth; i++)
	// {
	// 	for (int32 j = 0; j < GridHeight; j++)
	// 	{
	// 		AsyncTask(ENamedThreads::GameThread, [this, i, j]()
	// 		{
	// 			ATile* NewTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass());
	// 			SpawnTileToGrid(FIntPoint(i, j));
	// 		});
	// 	}
	// }
}

// Called every frame
void APuzzleGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 APuzzleGrid::GetIndex(int32 X, int32 Y)
{
	return Y * GridWidth + X;
}

void APuzzleGrid::SpawnTileToGrid(FIntPoint Coordinate)
{
	float TileSize = 100.f;
	if(TileClass)
	{
		FVector NewLocation = GetActorLocation() + FVector(Coordinate.X * TileSize, Coordinate.Y * TileSize, 0.0f);
		ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, NewLocation, FRotator::ZeroRotator);
		NewTile->SetCoord(Coordinate);
		TileGrid[GetIndex(Coordinate.X, Coordinate.Y)] = NewTile;
	}

}

void APuzzleGrid::ChangeTile(ATile* Tile_A, ATile* Tile_B)
{
	Tile_A->SetTileSelected();
	Tile_B->SetTileSelected();
	if(Tile_A != Tile_B)
	{
		int32 TileDist = abs(Tile_A->GetCoord().X - Tile_B->GetCoord().X) + abs(Tile_A->GetCoord().Y - Tile_B->GetCoord().Y);
		if(TileDist <= 1)
		{
			FIntPoint temp = Tile_A->GetCoord();
			Tile_A->SetTileLocation(Tile_B->GetCoord());
			Tile_B->SetTileLocation(temp);
		}
	}
}
