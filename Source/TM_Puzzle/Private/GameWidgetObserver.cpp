// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

#include "Components/TextBlock.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
	if (ScoreText) ScoreText->SetText(FText::AsNumber(UpdateScore));

}

void UGameWidgetObserver::OnNotifyRemainingMoves_Implementation(int32 RemainingMoves)
{
	if (RemainingMoveText) RemainingMoveText->SetText(FText::AsNumber(RemainingMoves));
}
