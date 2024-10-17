// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
	IObserver::OnNotify_Implementation(UpdateScore);
	
	CurrentScore = UpdateScore;

	UpdateScoreUI(CurrentScore);
}
