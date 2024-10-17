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
	
	int32 CurrentScore;

public:

	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateScoreUI(int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentScore() { return CurrentScore; }
};
