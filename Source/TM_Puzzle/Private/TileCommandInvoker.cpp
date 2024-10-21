// Fill out your copyright notice in the Description page of Project Settings.


#include "TileCommandInvoker.h"

#include "PuzzleGrid.h"
#include "SwapCommand.h"

// Sets default values
ATileCommandInvoker::ATileCommandInvoker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATileCommandInvoker::ExecuteCommand(class TSharedPtr<ICommand> Command)
{
	Command->Execute();
	CommandHistory.Push(Command);
}

void ATileCommandInvoker::UndoCommand()
{
	if(CommandHistory.Num())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,FString::Printf(TEXT("%s"), *Cast<SwapCommand>(CommandHistory.Last())->GridActor->GetName()));
		if(TSharedPtr<ICommand>& LastCommand = CommandHistory.Last())
		{
			LastCommand->Undo();
		}
		CommandHistory.Pop();
	}
}


