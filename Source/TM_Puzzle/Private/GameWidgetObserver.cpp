// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

#include "Puzzle_GameInstance.h"
#include "Components/TextBlock.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
	if(UPuzzle_GameInstance* PuzzleInstance = Cast<UPuzzle_GameInstance>(GetGameInstance()))
	{
		int32 CurrentScore = PuzzleInstance->GetCurrentScore();
		
		UpdateScoreUI(CurrentScore);
	}

}

void UGameWidgetObserver::UpdateScoreUI(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}
