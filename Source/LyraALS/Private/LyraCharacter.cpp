// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCharacter.h"
#include "Engine/DataTable.h"

// Sets default values
ALyraCharacter::ALyraCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALyraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALyraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALyraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ALyraCharacter::FireBulletManager(float& BulletAmount)
{
	if (BulletAmount > 0)
	{
		BulletAmount--;
		return true;
	}
	return false;
}

void ALyraCharacter::ReloadFlipBulletManager(float& ClipAmount, const float ClipSize, float& BulletAmount)
{
	if (ClipAmount > 0)
	{
		ClipAmount--;
	}
	BulletAmount = ClipSize;
}
