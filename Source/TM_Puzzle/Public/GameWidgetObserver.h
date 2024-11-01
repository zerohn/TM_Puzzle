// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Observer.h"
#include "Blueprint/UserWidget.h"
#include "GameWidgetObserver.generated.h"

/**
 * 
 */
UCLASS()
class TM_PUZZLE_API UGameWidgetObserver : public UUserWidget, public IObserver
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainingMoveText;
public:

	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	virtual void OnNotifyRemainingMoves_Implementation(int32 RemainingMoves) override;

};
