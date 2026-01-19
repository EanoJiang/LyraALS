#include "AnimNotifyBatchToolModule.h"

#include "AnimNotifyBatchToolObject.h"
#include "Modules/ModuleManager.h"

#include "Framework/Docking/TabManager.h"
#include "LevelEditor.h"
#include "PropertyEditorModule.h"
#include "IToolMenusModule.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"

IMPLEMENT_MODULE(FAnimNotifyBatchToolModule, AnimNotifyBatchTool)

const FName FAnimNotifyBatchToolModule::TabName(TEXT("AnimNotifyBatchTool"));

void FAnimNotifyBatchToolModule::StartupModule()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        TabName,
        FOnSpawnTab::CreateRaw(this, &FAnimNotifyBatchToolModule::SpawnPluginTab))
        .SetDisplayName(FText::FromString(TEXT("Anim Notify Batch Tool")))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAnimNotifyBatchToolModule::RegisterMenus));

    if (UToolMenus::TryGet() && UToolMenus::IsToolMenuUIEnabled())
    {
        RegisterMenus();
    }
}

void FAnimNotifyBatchToolModule::ShutdownModule()
{
    UnregisterMenus();
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
    DetailsView.Reset();
    ToolObject.Reset();
}

TSharedRef<SDockTab> FAnimNotifyBatchToolModule::SpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    if (!ToolObject.IsValid())
    {
        ToolObject = TStrongObjectPtr<UAnimNotifyBatchToolObject>(NewObject<UAnimNotifyBatchToolObject>(GetTransientPackage()));
    }

    if (!DetailsView.IsValid())
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.bHideSelectionTip = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bAllowSearch = true;
        DetailsViewArgs.bShowOptions = false;
        DetailsViewArgs.bShowScrollBar = true;

        DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
        DetailsView->SetObject(ToolObject.Get());
    }

    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(8.0f)
            [
                DetailsView.ToSharedRef()
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(8.0f)
            [
                SNew(SUniformGridPanel)
                .SlotPadding(4.0f)
                + SUniformGridPanel::Slot(0, 0)
                [
                    SNew(SButton)
                    .Text(FText::FromString(TEXT("Apply")))
                    .OnClicked_Lambda([this]()
                    {
                        if (ToolObject.IsValid())
                        {
                            ToolObject->Apply();
                        }
                        return FReply::Handled();
                    })
                ]
            ]
        ];
}

void FAnimNotifyBatchToolModule::RegisterMenus()
{
    if (!UToolMenus::TryGet())
    {
        return;
    }

    FToolMenuOwnerScoped OwnerScoped(this);

    auto AddOpenEntry = [](UToolMenu* InMenu)
    {
        if (!InMenu)
        {
            return;
        }

        FToolMenuSection& Section = InMenu->FindOrAddSection(TEXT("WindowLayout"));
        Section.AddMenuEntry(
            TEXT("AnimNotifyBatchTool.Open"),
            FText::FromString(TEXT("Anim Notify Batch Tool")),
            FText::FromString(TEXT("Open Anim Notify Batch Tool")),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([]()
            {
                FGlobalTabmanager::Get()->TryInvokeTab(FAnimNotifyBatchToolModule::TabName);
            })));
    };

    AddOpenEntry(UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Window")));
    AddOpenEntry(UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Tools")));

    UToolMenus::Get()->RefreshAllWidgets();
}

void FAnimNotifyBatchToolModule::UnregisterMenus()
{
    UToolMenus::UnregisterOwner(this);
}
