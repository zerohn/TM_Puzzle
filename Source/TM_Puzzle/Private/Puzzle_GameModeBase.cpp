// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameModeBase.h"

#include "GameStateSubject.h"
#include "GameWidgetObserver.h"
#include "Kismet/GameplayStatics.h"

APuzzle_GameModeBase::APuzzle_GameModeBase()
{
	
}

void APuzzle_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	UGameStateSubject* ObserverGameState = NewObject<UGameStateSubject>();
	UGameWidgetObserver* ScoreWidget = CreateWidget<UGameWidgetObserver>(GetWorld(), LoadClass<UGameWidgetObserver>(nullptr, TEXT("/Game/UI/WBP_ScoreWidget.WBP_ScoreWidget_C")));
	if (ObserverGameState && ScoreWidget)
	{
		ScoreWidget->AddToViewport();
		ObserverGameState->RegisterObserver(ScoreWidget);
	}
}
