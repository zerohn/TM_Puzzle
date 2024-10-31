// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Puzzle_GameModeBase.generated.h"

class UGameWidgetObserver;
/**
 * 
 */
UCLASS()
class TM_PUZZLE_API APuzzle_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	APuzzle_GameModeBase();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<UGameWidgetObserver> MainWidgetClass;
};
