// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameModeBase.h"

#include "GameStateSubject.h"
#include "GameWidgetObserver.h"
#include "Puzzle_GameInstance.h"
#include "Kismet/GameplayStatics.h"

APuzzle_GameModeBase::APuzzle_GameModeBase()
{
	
}

void APuzzle_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	UPuzzle_GameInstance* PuzzleInstance = Cast<UPuzzle_GameInstance>(GetGameInstance());
	if(PuzzleInstance)
	{
		PuzzleInstance->ResetGameState();
	}
	
	UGameStateSubject* ObserverGameState = NewObject<UGameStateSubject>();

	if (PuzzleInstance)
	{
		PuzzleInstance->SetGameStateSubject(ObserverGameState);
	}

	if (MainWidgetClass)
	{
		UGameWidgetObserver* ScoreWidget = CreateWidget<UGameWidgetObserver>(GetWorld(), MainWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			ObserverGameState->RegisterObserver(ScoreWidget);
		}
	}
}
