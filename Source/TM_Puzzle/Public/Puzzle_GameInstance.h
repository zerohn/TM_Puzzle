// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Puzzle_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TM_PUZZLE_API UPuzzle_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzle_GameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
	int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data")
	int32 RemainingMove;
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void AddScore(int32 Points);
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void DecreaseMove();
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void ResetGameState();
};
