// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

//物品类型
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Health,
	Shield,
	PistolClip,
	RifleClip
};

//物品属性
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
	FName Name;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
	EItemType ItemType;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
	int32 Amount;
};

UCLASS()
class LYRAALS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	//物品——数据表行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (ExposeOnSpawn = true))
	FDataTableRowHandle Item;
	//Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	//平台
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Pad;
	//枪械提示
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* Weapon;
	//特效
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* NiagaraSystem;
	//PickUp胶囊体触发器
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* TriggerCapsule;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
