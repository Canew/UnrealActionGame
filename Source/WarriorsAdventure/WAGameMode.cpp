// Fill out your copyright notice in the Description page of Project Settings.

#include "WAGameMode.h"
#include "WASwordman.h"
#include "WAGothicKnight.h"
#include "WAPlayerController.h"

AWAGameMode::AWAGameMode()
{
	DefaultPawnClass = AWAGothicKnight::StaticClass();
	PlayerControllerClass = AWAPlayerController::StaticClass();
}