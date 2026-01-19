#include "AnimNotifyBatchToolObject.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSync.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "UObject/Package.h"

static bool NearlyEqualTime(float A, float B)
{
    return FMath::Abs(A - B) <= 0.0005f;
}

int32 UAnimNotifyBatchToolObject::GetOrAddNotifyTrackIndex(UAnimSequenceBase* Anim, FName TrackName) const
{
    if (!Anim)
    {
        return 0;
    }

    const bool bHasExplicitTrackName = !TrackName.IsNone();

    if (!bHasExplicitTrackName)
    {
        if (Anim->AnimNotifyTracks.Num() == 0)
        {
            FAnimNotifyTrack DefaultTrack;
            DefaultTrack.TrackName = TEXT("Notifies");
            DefaultTrack.TrackColor = FLinearColor::White;
            Anim->AnimNotifyTracks.Add(DefaultTrack);
        }
        return 0;
    }

    for (int32 TrackIndex = 0; TrackIndex < Anim->AnimNotifyTracks.Num(); ++TrackIndex)
    {
        if (Anim->AnimNotifyTracks[TrackIndex].TrackName == TrackName)
        {
            return TrackIndex;
        }
    }

    FAnimNotifyTrack NewTrack;
    NewTrack.TrackName = TrackName;
    NewTrack.TrackColor = FLinearColor::White;
    Anim->AnimNotifyTracks.Add(NewTrack);
    return Anim->AnimNotifyTracks.Num() - 1;
}

UClass* UAnimNotifyBatchToolObject::ResolveNotifyClass(const FAnimNotifyBatchRule& Rule) const
{
    if (Rule.TargetNotifyClass.IsValid())
    {
        return Rule.TargetNotifyClass.Get();
    }

    if (!Rule.TargetNotifyClass.IsNull())
    {
        return Rule.TargetNotifyClass.LoadSynchronous();
    }
    return nullptr;
}

void UAnimNotifyBatchToolObject::Apply()
{
    if (AnimAssets.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("AnimNotifyBatchTool: AnimAssets 为空，请先拖入动画资产"));
        return;
    }

    if (Rules.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("AnimNotifyBatchTool: Rules 为空，请先配置规则"));
        return;
    }

    TArray<UPackage*> PackagesToSave;

    for (UAnimSequenceBase* Anim : AnimAssets)
    {
        if (!Anim)
        {
            continue;
        }

        Anim->Modify();

        for (const FAnimNotifyBatchRule& Rule : Rules)
        {
            if (Rule.SourceNotifyName.IsNone())
            {
                continue;
            }

            ApplyToSingleAnim(Anim, Rule);
        }

        Anim->MarkPackageDirty();

        if (bAutoSave)
        {
            if (UPackage* Pkg = Anim->GetOutermost())
            {
                PackagesToSave.AddUnique(Pkg);
            }
        }
    }

    if (bAutoSave && PackagesToSave.Num() > 0)
    {
        FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
    }

    UE_LOG(LogTemp, Display, TEXT("AnimNotifyBatchTool: Apply 完成"));
}

void UAnimNotifyBatchToolObject::ApplyToSingleAnim(UAnimSequenceBase* Anim, const FAnimNotifyBatchRule& Rule)
{
    UClass* NotifyClass = ResolveNotifyClass(Rule);
    if (!NotifyClass || !NotifyClass->IsChildOf(UAnimNotify::StaticClass()))
    {
        UE_LOG(LogTemp, Error, TEXT("AnimNotifyBatchTool: 规则 Source=%s 目标 TargetNotifyClass 未设置或无效"), *Rule.SourceNotifyName.ToString());
        return;
    }

    const FName TargetNotifyName = NotifyClass->GetFName();
    const int32 TargetTrackIndex = GetOrAddNotifyTrackIndex(Anim, Rule.TargetNotifyTrackName);

    // 遍历现有 Notifies，找到 SourceNotifyName 的事件，并在同一时间点补 TargetNotify
    // 注意：这里使用 FAnimNotifyEvent 的常用字段（NotifyName / DisplayTime）
    const TArray<FAnimNotifyEvent>& Existing = Anim->Notifies;

    TArray<float> TimesToAdd;
    TimesToAdd.Reserve(Existing.Num());

    for (const FAnimNotifyEvent& Evt : Existing)
    {
        const bool bMatchByEventName = (Evt.NotifyName == Rule.SourceNotifyName);
        const bool bMatchByNotifyObjectName = (Evt.Notify && Evt.Notify->GetFName() == Rule.SourceNotifyName);
        const bool bMatchByNotifyClassName = (Evt.Notify && Evt.Notify->GetClass() && Evt.Notify->GetClass()->GetFName() == Rule.SourceNotifyName);

        if (bMatchByEventName || bMatchByNotifyObjectName || bMatchByNotifyClassName)
        {
            TimesToAdd.Add(Evt.GetTime());
        }
    }

    // 兼容 Sync Marker 作为 source（你截图里 FootSyncMarkers 轨上的 L / R）
    if (UAnimSequence* AnimSeq = Cast<UAnimSequence>(Anim))
    {
        for (const FAnimSyncMarker& Marker : AnimSeq->AuthoredSyncMarkers)
        {
            if (Marker.MarkerName == Rule.SourceNotifyName)
            {
                TimesToAdd.Add(Marker.Time);
            }
        }
    }

    if (TimesToAdd.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Source Notify '%s' not found in this animation."), *Anim->GetName(), *Rule.SourceNotifyName.ToString());
        return;
    }

    for (float Time : TimesToAdd)
    {
        bool bAlreadyExists = false;
        for (const FAnimNotifyEvent& Evt : Anim->Notifies)
        {
            const bool bTargetMatchByEventName = (Evt.NotifyName == TargetNotifyName);
            const bool bTargetMatchByNotifyObjectName = (Evt.Notify && Evt.Notify->GetFName() == TargetNotifyName);
            const bool bTargetMatchByNotifyClassName = (Evt.Notify && Evt.Notify->GetClass() && Evt.Notify->GetClass()->GetFName() == TargetNotifyName);

            if ((bTargetMatchByEventName || bTargetMatchByNotifyObjectName || bTargetMatchByNotifyClassName) && NearlyEqualTime(Evt.GetTime(), Time))
            {
                bAlreadyExists = true;
                break;
            }
        }

        if (bAlreadyExists)
        {
            continue;
        }

        FAnimNotifyEvent NewEvent;
        NewEvent.NotifyName = TargetNotifyName;
        NewEvent.SetTime(Time);
        NewEvent.Notify = NewObject<UAnimNotify>(Anim, NotifyClass, NAME_None, RF_Transactional);
        NewEvent.TrackIndex = TargetTrackIndex;

        Anim->Notifies.Add(NewEvent);

        UE_LOG(LogTemp, Display, TEXT("[%s] Add Notify: %s at %.3f"), *Anim->GetName(), *TargetNotifyName.ToString(), Time);
    }

    Anim->RefreshCacheData();
    Anim->PostEditChange();
}
