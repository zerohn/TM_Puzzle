// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameInstance.h"

#include "GameStateSubject.h"

UPuzzle_GameInstance::UPuzzle_GameInstance()
{
	PlayerScore = 0;
	RemainingMove = 20;
	GameStateSubjectInstance = nullptr;
}

void UPuzzle_GameInstance::AddScore(int32 Points)
{
	PlayerScore += Points;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("PlayerScore : %d"), PlayerScore));
	if (GameStateSubjectInstance)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			GameStateSubjectInstance->NotifyObservers(this);
		});
	}
}

void UPuzzle_GameInstance::DecreaseMove()
{
	if(RemainingMove > 0)
	{
		RemainingMove--;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Remaining Moves : %d"), RemainingMove));
		if (GameStateSubjectInstance)
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				GameStateSubjectInstance->NotifyObserversMoves(this);
			});
		}
	}
}

void UPuzzle_GameInstance::ResetGameState()
{
	PlayerScore = 0;
	RemainingMove = 20;
	if (GameStateSubjectInstance)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			GameStateSubjectInstance->NotifyObservers(this);
			GameStateSubjectInstance->NotifyObserversMoves(this);
		});
	}
}

void UPuzzle_GameInstance::SetGameStateSubject(UGameStateSubject* NewSubject)
{
	GameStateSubjectInstance = NewSubject;
}
