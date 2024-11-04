// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Puzzle_GameInstance.generated.h"

class UGameStateSubject;
/**
 * 
 */
UCLASS()
class TM_PUZZLE_API UPuzzle_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:	
	UPuzzle_GameInstance();
	
private:
	int32 PlayerScore;
	int32 RemainingMove;
	UGameStateSubject* GameStateSubjectInstance;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void AddScore(int32 Points);
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void DecreaseMove();
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void ResetGameState();
	
	int32 GetCurrentScore() const { return PlayerScore; }
	int32 GetRemainingMove() const { return RemainingMove; }

	void SetGameStateSubject(UGameStateSubject* NewSubject);
	void RemoveGameStateSubject();
};
