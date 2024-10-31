// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UGameStateSubject::UGameStateSubject()
{
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Add(Observer);
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Remove(Observer);
}

void UGameStateSubject::NotifyObservers(UObject* WorldContextObject)
{
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		if(Observer.GetObject() && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			IObserver::Execute_OnNotify(Observer.GetObject(), PlayerScore);
		}
	}
}