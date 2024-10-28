// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "GameFramework/Actor.h"
#include "TileCommandInvoker.generated.h"

UCLASS()
class TM_PUZZLE_API ATileCommandInvoker : public AActor
{
	GENERATED_BODY()
public:
	TArray<TSharedPtr<ICommand>> CommandHistory;
public:	
	// Sets default values for this actor's properties
	ATileCommandInvoker();

	void ExecuteCommand(class TSharedPtr<ICommand> Command);
	void UndoCommand();
	void ClearHistory();
};
