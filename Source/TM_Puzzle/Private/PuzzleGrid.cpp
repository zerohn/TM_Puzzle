// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGrid.h"

#include "SwapCommand.h"
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

	CommandInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>(ATileCommandInvoker::StaticClass());
	
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
	if (CurrentPuzzleState == NewPuzzleState)
	{
		return;
	}

	CurrentPuzzleState = NewPuzzleState;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "ChangePuzzleState");
	FTimerHandle PuzzleTimerHandle;
	GetWorldTimerManager().SetTimer(PuzzleTimerHandle, this, &APuzzleGrid::HandlePuzzleState, 0.5f);
}

void APuzzleGrid::HandlePuzzleState()
{
	switch (CurrentPuzzleState)
	{
	case EPuzzleState::Main:
		break;
	case EPuzzleState::Start:
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "PuzzleState : Start");
		InitPuzzleGrid();
		break;
	case EPuzzleState::Idle:
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "PuzzleState : Idle");
		break;
	case EPuzzleState::Move:
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "PuzzleState : Move");
		ChangeTile();
		break;
	case EPuzzleState::Check:
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "PuzzleState : Check");
		break;
	case EPuzzleState::Pop:
		break;
	case EPuzzleState::Spawn:
		break;
	case EPuzzleState::End:
		break;
	}
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
			if (TileClass.Num())
			{
				float TileSize = 100.f;

				FVector NewLocation = GetActorLocation() + FVector(i * TileSize, j * TileSize, 0.0f);
				// ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation, FRotator::ZeroRotator);
				// PuzzleGrid[GetIndex(j, i)] = NewTile;
				AsyncTask(ENamedThreads::GameThread, [this, i, j, NewLocation]()
				{
					ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation,
					                                               FRotator::ZeroRotator);
					PuzzleGrid[GetIndex(i, j)] = NewTile;
				});
			}
		}
	}
	SetPuzzleState(EPuzzleState::Idle);
}

void APuzzleGrid::AddSelectedTile(ATile* NewTile)
{
	int32 idx;
	PuzzleGrid.Find(NewTile, idx);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("idx : %d"), idx));
	UE_LOG(LogTemp, Warning, TEXT("idx : %d"), idx);
	
	if (CurrentPuzzleState == EPuzzleState::Idle)
	{
		if (SelectedTile[0])
		{
			if (SelectedTile[0] != NewTile)
			{
				NewTile->ChangeTileSelected();
				SelectedTile[1] = NewTile;
				SetPuzzleState(EPuzzleState::Move);
			}
			else
			{
				SelectedTile[0]->ChangeTileSelected();
				SelectedTile[0] = nullptr;
			}
		}
		else
		{
			NewTile->ChangeTileSelected();
			SelectedTile[0] = NewTile;
		}
	}
}

bool APuzzleGrid::bCanChangeTile(ATile* Tile_A, ATile* Tile_B)
{
	int32 idx_1;
	int32 idx_2;
	if (PuzzleGrid.Find(Tile_A, idx_1) && PuzzleGrid.Find(Tile_B, idx_2))
	{
		if (abs(GetCoordinate(idx_1).X - GetCoordinate(idx_2).X) + abs(GetCoordinate(idx_1).Y - GetCoordinate(idx_2).Y)
			== 1)
		{
			return true;
		}
	}
	return false;
}

void APuzzleGrid::ChangeTile()
{
	if(bCanChangeTile(SelectedTile[0], SelectedTile[1]))
	{
		Alpha = 0.0f;
		FVector Loc_A = SelectedTile[0]->GetActorLocation();
		FVector Loc_B = SelectedTile[1]->GetActorLocation();
		GetWorldTimerManager().SetTimer(TileAnimHandle, [this, Loc_A, Loc_B]()
		{
			ChangeAnimation(Loc_A, Loc_B);
		}, 0.02f, true);
	}
	else
	{
		SelectedTile[0]->ChangeTileSelected();
		SelectedTile[1]->ChangeTileSelected();
		SelectedTile[0] = nullptr;
		SelectedTile[1] = nullptr;
		SetPuzzleState(EPuzzleState::Idle);
	}
}

void APuzzleGrid::ChangeAnimation(const FVector Loc_A, const FVector Loc_B)
{
	SelectedTile[0]->SetActorLocation(FMath::Lerp(Loc_A, Loc_B, Alpha));
	SelectedTile[1]->SetActorLocation(FMath::Lerp(Loc_B, Loc_A, Alpha));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Alpha : %.f"), Alpha));
	Alpha += 0.05f;
	if (Alpha > 1.0f)
	{
		Alpha = 0.f;
		GetWorldTimerManager().ClearTimer(TileAnimHandle);
		SelectedTile[0]->ChangeTileSelected();
		SelectedTile[0]->SetActorLocation(Loc_B);
		SelectedTile[1]->ChangeTileSelected();
		SelectedTile[1]->SetActorLocation(Loc_A);
		UpdateTileIndex();
	}
}

void APuzzleGrid::UpdateTileIndex()
{
	SwapCommand* NewCommand = new SwapCommand(SelectedTile[0], SelectedTile[1]);
	CommandInvoker->ExecuteCommand(NewCommand);
	SelectedTile[0] = nullptr;
	SelectedTile[1] = nullptr;
	SetPuzzleState(EPuzzleState::Idle);
}

void APuzzleGrid::SpawnTileToGrid(FIntPoint Coordinate)
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
			if (PuzzleGrid[j])
			{
				if (MatchingTileIdx.IsEmpty())
				{
					MatchingTileIdx.Add(j);
				}
				else
				{
					if (PuzzleGrid[MatchingTileIdx.Last()]->GetTileType() == PuzzleGrid[j]->GetTileType())
					{
						MatchingTileIdx.Add(j);
					}
					else
					{
						MatchingTileIdx.Reset();
						MatchingTileIdx.Add(j);
					}
				}
				if (MatchingTileIdx.Num() >= 3)
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
			if (PuzzleGrid[j])
			{
				if (MatchingTileIdx.IsEmpty())
				{
					MatchingTileIdx.Add(j);
				}
				else
				{
					if (PuzzleGrid[MatchingTileIdx.Last()]->GetTileType() == PuzzleGrid[j]->GetTileType())
					{
						MatchingTileIdx.Add(j);
					}
					else
					{
						MatchingTileIdx.Reset();
						MatchingTileIdx.Add(j);
					}
				}
				if (MatchingTileIdx.Num() >= 3)
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
		if (PuzzleGrid[i] && PuzzleGrid[i]->bIsMatched)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("MatchingTile : %s"), *PuzzleGrid[i]->GetName()));
			PuzzleGrid[i]->Destroy();
			PuzzleGrid[i] = nullptr;
		}
	}
}
