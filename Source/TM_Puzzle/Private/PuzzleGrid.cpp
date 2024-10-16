// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGrid.h"
#include "Tile.h"
#include "TileCommandInvoker.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APuzzleGrid::APuzzleGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentPuzzleState = EPuzzleState::Main;
}

// Called when the game starts or when spawned
void APuzzleGrid::BeginPlay()
{
	Super::BeginPlay();

	// Set Begin State
	SetPuzzleState(EPuzzleState::Start);

}

// Called every frame
void APuzzleGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuzzleGrid::SetPuzzleState(const EPuzzleState NewPuzzleState)
{
	if (CurrentPuzzleState == NewPuzzleState) return;

	CurrentPuzzleState = NewPuzzleState;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "ChangePuzzleState");
	FTimerHandle PuzzleTimerHandle;
	GetWorldTimerManager().SetTimer(PuzzleTimerHandle, this, &APuzzleGrid::HandlePuzzleState, 1.5f);
}

void APuzzleGrid::HandlePuzzleState()
{
	switch (CurrentPuzzleState)
	{
	case EPuzzleState::Main:
		break;
	case EPuzzleState::Start:
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "PuzzleState : Start");
		InitPuzzleGrid();
		break;
	case EPuzzleState::Idle:
		break;
	case EPuzzleState::Move:
		break;
	case EPuzzleState::Check:
		break;
	case EPuzzleState::Pop:
		break;
	case EPuzzleState::Spawn:
		break;
	case EPuzzleState::End:
		break;
	}
}

void APuzzleGrid::AddSelectedTile(ATile* NewTile)
{
	if(SelectedTile)
	{
		if(SelectedTile != NewTile || bCanChangeTile(SelectedTile, NewTile))
		{
			
		}
	}
	else
	{
		SelectedTile = NewTile;
	}
}

bool APuzzleGrid::bCanChangeTile(ATile* Tile_A, ATile* Tile_B)
{
	int32 idx_1;
	int32 idx_2;
	if(PuzzleGrid.Find(Tile_A, idx_1) && PuzzleGrid.Find(Tile_B, idx_2))
	{
		if(abs(GetCoordinate(idx_1).X - GetCoordinate(idx_2).X) + abs(GetCoordinate(idx_1).Y - GetCoordinate(idx_2).Y) <= 1)
		{
			return true;
		}
	}
	return false;
}

int32 APuzzleGrid::GetIndex(int32 X, int32 Y)
{
	return Y * GridWidth + X;
}

FIntPoint APuzzleGrid::GetCoordinate(int32 idx)
{
	return FIntPoint(idx % GridWidth, idx / GridWidth);
}

TSubclassOf<class ATile> APuzzleGrid::GetRandomTileClass()
{
	int32 RandIdx = UKismetMathLibrary::RandomIntegerInRange(0, TileClass.Num() - 1);
	return TileClass[RandIdx];
}

void APuzzleGrid::InitPuzzleGrid()
{
	// Set PuzzleGrid Size
	PuzzleGrid.Reset();
	PuzzleGrid.SetNum(GridHeight * GridWidth);
	
	for (int32 i = 0; i < GridHeight; i++)
	{
		for (int32 j = 0; j < GridWidth; j++)
		{
			if(TileClass.Num())
			{
				float TileSize = 100.f;

				FVector NewLocation = GetActorLocation() + FVector(j * TileSize, i * TileSize, 0.0f);
				// ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation, FRotator::ZeroRotator);
				// PuzzleGrid[GetIndex(j, i)] = NewTile;
				AsyncTask(ENamedThreads::GameThread, [this, i, j, NewLocation]()
				{
					ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation, FRotator::ZeroRotator);
					PuzzleGrid[GetIndex(j, i)] = NewTile;
				});
			}
		}
	}
	SetPuzzleState(EPuzzleState::Idle);
}

void APuzzleGrid::SpawnTileToGrid(FIntPoint Coordinate)
{
}

void APuzzleGrid::ChangeTile(ATile* Tile_A, ATile* Tile_B)
{
	
}

bool APuzzleGrid::MatchingCheck()
{
	bool bIsMatch = false;
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Matching Check")));
	// 가로 탐색
	MatchingTileIdx.Reset();
	for (int i = 0; i < GridHeight; i++)
	{
		for (int j = i * GridWidth; j < i * GridWidth + GridWidth; j++)
		{
			if(PuzzleGrid[j])
			{
				if(MatchingTileIdx.IsEmpty())
				{
					MatchingTileIdx.Add(j);
				}
				else
				{
					if(PuzzleGrid[MatchingTileIdx.Last()]->GetTileType() == PuzzleGrid[j]->GetTileType())
					{
						MatchingTileIdx.Add(j);
					}
					else
					{
						MatchingTileIdx.Reset();
						MatchingTileIdx.Add(j);
					}
				}
				if(MatchingTileIdx.Num() >= 3)
				{
					for (int k = 0; k < MatchingTileIdx.Num(); k++)
					{
						PuzzleGrid[MatchingTileIdx[k]]->bIsMatched = true;
						bIsMatch = true;
					}
				}
			}
			else
			{
				MatchingTileIdx.Reset();
			}
		}
		MatchingTileIdx.Reset();
	}
	// 세로 탐색
	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = i; j < GridHeight * GridWidth; j += GridHeight)
		{
			if(PuzzleGrid[j])
			{
				if(MatchingTileIdx.IsEmpty())
				{
					MatchingTileIdx.Add(j);
				}
				else
				{
					if(PuzzleGrid[MatchingTileIdx.Last()]->GetTileType() == PuzzleGrid[j]->GetTileType())
					{
						MatchingTileIdx.Add(j);
					}
					else
					{
						MatchingTileIdx.Reset();
						MatchingTileIdx.Add(j);
					}
				}
				if(MatchingTileIdx.Num() >= 3)
				{
					for (int k = 0; k < MatchingTileIdx.Num(); k++)
					{
						PuzzleGrid[MatchingTileIdx[k]]->bIsMatched = true;
						bIsMatch = true;
					}
				}
			}
			else
			{
				MatchingTileIdx.Reset();
			}
		}
		MatchingTileIdx.Reset();
	}
	return bIsMatch;
}

void APuzzleGrid::PopTile()
{
	for (int i = 0; i < PuzzleGrid.Num(); i++)
	{
		if(PuzzleGrid[i] && PuzzleGrid[i]->bIsMatched)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("MatchingTile : %s"), *PuzzleGrid[i]->GetName()));
			PuzzleGrid[i]->Destroy();
			PuzzleGrid[i] = nullptr;
		}
	}
}
