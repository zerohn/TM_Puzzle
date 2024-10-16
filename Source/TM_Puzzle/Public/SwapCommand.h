// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "Tile.h"

/**
 * 
 */
class TM_PUZZLE_API SwapCommand : public ICommand
{
public:
	SwapCommand();
	SwapCommand(ATile* InTile_A, ATile* InTile_B) : Tile_A(InTile_A), Tile_B(InTile_B) {}
	~SwapCommand();

	virtual void Execute() override;
	virtual void Undo() override;
private:
	ATile* Tile_A;
	ATile* Tile_B;
};
