// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapCommand.h"

#include "PuzzleGrid.h"

void SwapCommand::Execute()
{
	GridActor->SwapTile(Tile_A, Tile_B);
}

void SwapCommand::Undo()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("Undo %s, %s"), *Tile_A->GetName(), *Tile_B->GetName()));
	GridActor->SwapTile(Tile_A, Tile_B);
	Tile_A->SetTileSelected(false);
	Tile_B->SetTileSelected(false);
}
