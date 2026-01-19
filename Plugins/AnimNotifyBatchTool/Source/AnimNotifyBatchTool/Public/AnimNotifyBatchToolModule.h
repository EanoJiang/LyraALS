#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IDetailsView;
class UAnimNotifyBatchToolObject;

class FAnimNotifyBatchToolModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterMenus();
    void UnregisterMenus();

    TSharedRef<class SDockTab> SpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
    static const FName TabName;

private:
    TStrongObjectPtr<UAnimNotifyBatchToolObject> ToolObject;
    TSharedPtr<IDetailsView> DetailsView;
};
