// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LyraCharacter.generated.h"

UCLASS()
class LYRAALS_API ALyraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float Health = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth = 250;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float Shield = 40;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxShield = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	ALyraCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Fire时的弹药管理逻辑
	UFUNCTION(BlueprintCallable, Category="Bullet")
	bool FireBulletManager(UPARAM(ref) float& BulletAmount);

	//Reload时的弹药管理逻辑
	UFUNCTION(BlueprintCallable, Category="Bullet")
	void ReloadFlipBulletManager(UPARAM(ref) float& ClipAmount, float ClipSize,UPARAM(ref) float& BulletAmount);

	//更新增加生命值
	UFUNCTION(BlueprintCallable, Category="Health")
	void UpdateIncreaseHealth(UPARAM(ref) float& DelHealth);
	//更新减少生命值
	UFUNCTION(BlueprintCallable, Category="Health")
	void UpdateDecreaseHealth(UPARAM(ref) float& DelHealth);
	//更新增加护盾
	UFUNCTION(BlueprintCallable, Category="Health")
	void UpdateIncreaseShield(UPARAM(ref) float& DelShield);
	
};
