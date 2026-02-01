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
	//Health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float Health = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth = 250;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float Shield = 40;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxShield = 100;
	//弹药
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pistol)
	float MaxPistolClipAmount = 7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pistol)
	float PistolClipAmount = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pistol)
	float PistolClipSize = 12;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pistol)
	float PistolBulletAmount = 7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rifle)
	float MaxRifleClipAmount = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rifle)
	float RifleClipAmount = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rifle)
	float RifleClipSize = 80;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Rifle)
	float RifleBulletAmount = 60;

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

	//更新增加手枪弹夹
	UFUNCTION(BlueprintCallable, Category="Clip")
	void UpdateIncreasePistolClip(UPARAM(ref) int& DelClip);
	//更新增加长枪弹夹
	UFUNCTION(BlueprintCallable, Category="Clip")
	void UpdateIncreaseRifleClip(UPARAM(ref) int& DelClip);
	
	
	
};
