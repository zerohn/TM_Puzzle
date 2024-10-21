// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "Tile.h"

class APuzzleGrid;
/**
 * 
 */
class TM_PUZZLE_API SwapCommand : public ICommand
{
public:
	SwapCommand(APuzzleGrid* InPuzzleGrid, ATile* InTile_A, ATile* InTile_B) : GridActor(InPuzzleGrid), Tile_A(InTile_A), Tile_B(InTile_B) {}
	
	virtual void Execute() override;
	virtual void Undo() override;

public:
	APuzzleGrid* GridActor;
	ATile* Tile_A;
	ATile* Tile_B;
};
