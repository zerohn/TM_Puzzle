// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameInstance.h"

#include "GameStateSubject.h"

UPuzzle_GameInstance::UPuzzle_GameInstance()
{
	PlayerScore = 0;
	RemainingMove = 30;
	GameStateSubjectInstance = nullptr;
}

void UPuzzle_GameInstance::AddScore(int32 Points)
{
	PlayerScore += Points;

	if (GameStateSubjectInstance)
	{
		GameStateSubjectInstance->NotifyObservers();
	}
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
	PlayerScore = 0;
	RemainingMove = 30;
}

void UPuzzle_GameInstance::SetGameStateSubject(UGameStateSubject* NewSubject)
{
	GameStateSubjectInstance = NewSubject;
}
