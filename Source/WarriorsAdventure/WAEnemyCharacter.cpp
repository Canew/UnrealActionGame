// Fill out your copyright notice in the Description page of Project Settings.


#include "WAEnemyCharacter.h"

AWAEnemyCharacter::AWAEnemyCharacter()
{
	
	// Set collision channel.
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("WAEnemyCharacter"));
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6);
}