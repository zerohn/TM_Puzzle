// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapCommand.h"

#include "PuzzleGrid.h"

void SwapCommand::Execute()
{
	GridActor->ChangeTile(Tile_A, Tile_B);
	GridActor->PuzzleGrid.Swap(GridActor->PuzzleGrid.Find(Tile_A), GridActor->PuzzleGrid.Find(Tile_B));
}

void SwapCommand::Undo()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("Undo %s, %s"), *Tile_A->GetName(), *Tile_B->GetName()));
	GridActor->ChangeTile(Tile_A, Tile_B);
	GridActor->PuzzleGrid.Swap(GridActor->PuzzleGrid.Find(Tile_A), GridActor->PuzzleGrid.Find(Tile_B));
	Tile_A->ChangeTileSelected();
	Tile_B->ChangeTileSelected();
}
