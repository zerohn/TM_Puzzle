// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle_GameModeBase.h"

#include "Kismet/GameplayStatics.h"

APuzzle_GameModeBase::APuzzle_GameModeBase()
{
	
}

void APuzzle_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}
