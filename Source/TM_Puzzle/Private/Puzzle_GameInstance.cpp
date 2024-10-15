// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameInstance.h"

UPuzzle_GameInstance::UPuzzle_GameInstance()
{
	Score = 0;
	RemainingMove = 30;
}

void UPuzzle_GameInstance::AddScore(int32 Points)
{
	Score += Points;
}

void UPuzzle_GameInstance::DecreaseMove()
{
	if(RemainingMove > 0)
	{
		RemainingMove--;
	}
}

void UPuzzle_GameInstance::ResetGameState()
{
	Score = 0;
	RemainingMove = 30;
}
