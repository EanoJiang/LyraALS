#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AnimNotifyBatchToolObject.generated.h"

class UAnimSequenceBase;
class UAnimSequence;
class UAnimNotify;

USTRUCT(BlueprintType)
struct FAnimNotifyBatchRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    FName SourceNotifyName;

    // 目标 Notify 类：可在这里拖入 AnimNotify 的 Blueprint Class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    TSoftClassPtr<UAnimNotify> TargetNotifyClass;

    // 新增的 Notify 放到哪个 Track（Notify Track 名称）。为空则使用默认 Track(0)。
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    FName TargetNotifyTrackName;
};

UCLASS(BlueprintType)
class UAnimNotifyBatchToolObject : public UObject
{
    GENERATED_BODY()

public:
    // 待处理动画资产（支持批量拖入）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    TArray<TObjectPtr<UAnimSequenceBase>> AnimAssets;

    // 规则列表：例如 Source=L -> Target=ANC_LeftFootOnGround (Notify类)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    TArray<FAnimNotifyBatchRule> Rules;

    // 是否自动保存资产
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyBatchTool")
    bool bAutoSave = false;

    void Apply();

private:
    UClass* ResolveNotifyClass(const FAnimNotifyBatchRule& Rule) const;
    void ApplyToSingleAnim(UAnimSequenceBase* Anim, const FAnimNotifyBatchRule& Rule);
    int32 GetOrAddNotifyTrackIndex(UAnimSequenceBase* Anim, FName TrackName) const;
};
