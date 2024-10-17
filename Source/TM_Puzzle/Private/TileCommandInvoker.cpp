// Fill out your copyright notice in the Description page of Project Settings.


#include "TileCommandInvoker.h"

// Sets default values
ATileCommandInvoker::ATileCommandInvoker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATileCommandInvoker::ExecuteCommand(class ICommand* Command)
{
	Command->Execute();
	CommandHistory.Push(Command);
}

void ATileCommandInvoker::UndoCommand()
{
	if(CommandHistory.Num())
	{
		ICommand* LastCommand = CommandHistory.Last();
		LastCommand->Undo();

		CommandHistory.Pop();
	}
}


