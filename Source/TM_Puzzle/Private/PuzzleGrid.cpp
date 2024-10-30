// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGrid.h"

#include "Puzzle_GameModeBase.h"
#include "SwapCommand.h"
#include "Tile.h"
#include "TileCommandInvoker.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APuzzleGrid::APuzzleGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CurrentPuzzleState = EPuzzleState::MainMenu;
}

// Called when the game starts or when spawned
void APuzzleGrid::BeginPlay()
{
	Super::BeginPlay();

	CommandInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>(ATileCommandInvoker::StaticClass());
	
	// Set Begin State
	SetPuzzleState(EPuzzleState::Initialize);
	MoveCounter.Set(0);
}

// Called every frame
void APuzzleGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuzzleGrid::SetPuzzleState(const EPuzzleState NewPuzzleState)
{
	CurrentPuzzleState = NewPuzzleState;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, "ChangePuzzleState");
	FTimerHandle PuzzleTimerHandle;
	GetWorldTimerManager().SetTimer(PuzzleTimerHandle, this, &APuzzleGrid::HandlePuzzleState, 0.5f);
}

void APuzzleGrid::HandlePuzzleState()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("State : %s"), *StaticEnum<EPuzzleState>()->GetValueAsString(CurrentPuzzleState)));
	switch (CurrentPuzzleState)
	{
	case EPuzzleState::MainMenu:
		{
			// 메인 메뉴 (UI)
			break;
		}
	case EPuzzleState::Initialize:
		{
			// 퍼즐 게임 시작 전 준비 과정
			InitPuzzleGrid();
			SetPuzzleState(EPuzzleState::PlayerTurn);
			break;
		}
	case EPuzzleState::PlayerTurn:
		{
			// 플레이어의 입력 대기 상태
			break;
		}
	case EPuzzleState::ResolveBoard:
		{
			// 퍼즐의 매칭상태 확인, 타일 소멸, 타일 생성 등 메인 게임 로직
			
			// 매칭 된 타일이 있다면 타일 소멸 함수 실행, 없다면 Undo Command 실행
			if(MatchingCheck())
			{
				CommandInvoker->ClearHistory();
				PopTile();
				DropTile();
				SpawnTileToGrid();
			}
			else
			{
				CommandInvoker->UndoCommand();
				SetPuzzleState(EPuzzleState::PlayerTurn);
			}
			
			// 타일 2개가 선택되어 있을 시 SwapCommand 실행
			if (SelectedTile[0] != nullptr && SelectedTile[1] != nullptr)
			{
				TSharedPtr<SwapCommand> NewCommand = MakeShared<SwapCommand>(this, SelectedTile[0], SelectedTile[1]);
				CommandInvoker->ExecuteCommand(NewCommand);
				SelectedTile[0] = nullptr;
				SelectedTile[1] = nullptr;
			}
			//DropTile();
			break;
		}
	case EPuzzleState::GameOver:
		{
			// 게임오버 상태 (UI)
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Invoker History Size : %d"), CommandInvoker->CommandHistory.Num()));
			
			break;
		}
	}
}

int32 APuzzleGrid::GetIndex(int32 X, int32 Y)
{
	return Y * GridWidth + X;
}

FIntPoint APuzzleGrid::GetCoordinate(int32 idx)
{
	return FIntPoint(idx / GridWidth, idx % GridWidth);
}

TSubclassOf<class ATile> APuzzleGrid::GetRandomTileClass()
{
	int32 RandIdx = UKismetMathLibrary::RandomIntegerInRange(0, TileClass.Num() - 1);
	return TileClass[RandIdx];
}

void APuzzleGrid::InitPuzzleGrid()
{
	// Set PuzzleGrid Size
	PuzzleGrid.SetNum(GridHeight * GridWidth);

	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			if (TileClass.Num() && PuzzleGrid[GetIndex(x, y)] == nullptr)
			{
				FVector NewLocation = GetActorLocation() + FVector(y * TileSize, x * TileSize, 0.0f);
				ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation, FRotator::ZeroRotator);
				PuzzleGrid[GetIndex(x, y)] = NewTile;
				// AsyncTask(ENamedThreads::GameThread, [this, x, y, NewLocation]()
				// {
				// 	ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass(), NewLocation,
				// 	                                               FRotator::ZeroRotator);
				// 	PuzzleGrid[GetIndex(x, y)] = NewTile;
				// });
			}
		}
	}
	if (MatchingCheck())
	{
		PopTile();
		InitPuzzleGrid();
	}
}

void APuzzleGrid::AddSelectedTile(ATile* NewTile)
{
	if (CurrentPuzzleState != EPuzzleState::PlayerTurn) return;
	
	// int32 idx;
	// PuzzleGrid.Find(NewTile, idx);
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("idx : %d"), idx));
	
	if (SelectedTile[0])
	{
		if (SelectedTile[0] != NewTile)
		{
			NewTile->SetTileSelected(true);
			SelectedTile[1] = NewTile;
			SetPuzzleState(EPuzzleState::ResolveBoard);
		}
		else
		{
			SelectedTile[0]->SetTileSelected(false);
			SelectedTile[0] = nullptr;
		}
	}
	else
	{
		NewTile->SetTileSelected(true);
		SelectedTile[0] = NewTile;
	}
}

bool APuzzleGrid::bCanSwapTile(ATile* Tile_A, ATile* Tile_B)
{
	int32 idx_1;
	int32 idx_2;
	if (PuzzleGrid.Find(Tile_A, idx_1) && PuzzleGrid.Find(Tile_B, idx_2))
	{
		if (abs(GetCoordinate(idx_1).X - GetCoordinate(idx_2).X) + abs(GetCoordinate(idx_1).Y - GetCoordinate(idx_2).Y) == 1)
		{
			return true;
		}
	}
	return false;
}

void APuzzleGrid::SwapTile(ATile* Tile_A, ATile* Tile_B)
{
	if(bCanSwapTile(Tile_A, Tile_B))
	{
		Alpha = 0.0f;
		FVector Loc_A = Tile_A->GetActorLocation();
		FVector Loc_B = Tile_B->GetActorLocation();
		Tile_A->MoveToLocation(Loc_B, 0.5f);
		Tile_B->MoveToLocation(Loc_A, 0.5f);
		// GetWorldTimerManager().SetTimer(TileAnimHandle, [this, Tile_A, Tile_B, Loc_A, Loc_B]()
		// {
		// 	ChangeAnimation(Tile_A, Tile_B, Loc_A, Loc_B);
		// }, 0.01f, true);
		PuzzleGrid.Swap(PuzzleGrid.Find(Tile_A), PuzzleGrid.Find(Tile_B));
	}
	else
	{
		SelectedTile[0]->SetTileSelected(false);
		SelectedTile[0] = nullptr;
		SelectedTile[1]->SetTileSelected(false);
		SelectedTile[1] = nullptr;
		SetPuzzleState(EPuzzleState::PlayerTurn);
	}
}

void APuzzleGrid::DropTile()
{
	//타일의 새로 방향을 확인하며 빈 공간으로 타일 이동
	TArray<TPair<ATile*, int32>> MoveTiles;
	for (int i = 0; i < GridHeight * GridWidth; i++)
	{
		if (PuzzleGrid[i] == nullptr)
		{
			for (int j = i + GridWidth; j < GridHeight * GridWidth; j += GridWidth)
			{
				if (PuzzleGrid[j] != nullptr)
				{
					MoveTiles.Add(TPair<ATile*, int32>(PuzzleGrid[j], i));
					PuzzleGrid[j] = nullptr;
					break;
				}
			}
		}
	}
	for (auto Tile : MoveTiles)
	{
		PuzzleGrid[Tile.Value] = Tile.Key;
		FIntPoint Coord = GetCoordinate(Tile.Value);
		Tile.Key->MoveToLocation(GetActorLocation() + FVector(Coord.X * TileSize, Coord.Y * TileSize, 0.0f), 0.5f);
	}
}

// void APuzzleGrid::ChangeAnimation(ATile* Tile_A, ATile* Tile_B, const FVector Loc_A, const FVector Loc_B)
// {
// 	Tile_A->SetActorLocation(FMath::Lerp(Loc_A, Loc_B, Alpha));
// 	Tile_B->SetActorLocation(FMath::Lerp(Loc_B, Loc_A, Alpha));
// 	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Alpha : %.2f"), Alpha));
// 	Alpha += 0.05f;
// 	if (Alpha > 1.0f)
// 	{
// 		Alpha = 0.f;
// 		GetWorldTimerManager().ClearTimer(TileAnimHandle);
// 		Tile_A->SetActorLocation(Loc_B);
// 		Tile_B->SetActorLocation(Loc_A);
// 		SetPuzzleState(EPuzzleState::ResolveBoard);
// 		//UpdateTileIndex();
// 	}
// }

void APuzzleGrid::SpawnTileToGrid()
{
	for (int32 i = 0; i < PuzzleGrid.Num(); i++)
	{
		if (PuzzleGrid[i] == nullptr)
		{
			FIntPoint Coord = GetCoordinate(i);
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(GetRandomTileClass());
			NewTile->SetActorLocation(GetActorLocation() + FVector(Coord.X * TileSize + GridHeight * TileSize, Coord.Y * TileSize, 0.0f));
			PuzzleGrid[i] = NewTile;
			NewTile->MoveToLocation(GetActorLocation() + FVector(Coord.X * TileSize, Coord.Y * TileSize, 0.0f), 0.5f);
		}
	}
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
		for (int j = i; j < GridHeight * GridWidth; j += GridWidth)
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

void APuzzleGrid::StartMoveAnim()
{
	MoveCounter.Increment();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("MoveCount : %d"), MoveCounter.GetValue()));
}

void APuzzleGrid::EndMoveAnim()
{
	MoveCounter.Decrement();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("MoveCount : %d"), MoveCounter.GetValue()));

	if (MoveCounter.GetValue() == 0)
	{
		SetPuzzleState(EPuzzleState::ResolveBoard);
	}
}
