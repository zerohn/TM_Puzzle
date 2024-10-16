// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapCommand.h"

SwapCommand::SwapCommand()
{
}

SwapCommand::~SwapCommand()
{
}

void SwapCommand::Execute()
{
	ATile* Temp = Tile_A;
	Tile_A = Tile_B;
	Tile_B = Temp;
}

void SwapCommand::Undo()
{
	ATile* Temp = Tile_A;
	Tile_A = Tile_B;
	Tile_B = Temp;
}
