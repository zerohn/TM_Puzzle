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

public:

	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScoreUI(int32 NewScore);
};
