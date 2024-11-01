// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Observer.h"
#include "GameStateSubject.generated.h"

/**
 * 
 */
UCLASS()
class TM_PUZZLE_API UGameStateSubject : public UObject
{
	GENERATED_BODY()
public:

	UGameStateSubject();
	
private:
	
	TArray<TScriptInterface<IObserver>> Observers;
	
public:

	void RegisterObserver(TScriptInterface<IObserver> Observer);

	void UnregisterObserver(TScriptInterface<IObserver> Observer);

	void NotifyObservers(UObject* WorldContextObject);

	void NotifyObserversMoves(UObject* WorldContextObject);
};
