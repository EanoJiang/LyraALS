/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "ConnectionDrawingPolicy.h"
#include "../Public/ElectronicNodesSettings.h"

#include "BlueprintConnectionDrawingPolicy.h"

class FENPathDrawer;

struct ENRibbonConnection
{
	float Main;
	float Sub;
	bool Horizontal;
	float Start;
	float End;
	int32 Depth = 0;

	ENRibbonConnection(float Main, float Sub, bool Horizontal, float Start, float End, int32 Depth = 0)
	{
		this->Main = Main;
		this->Sub = Sub;
		this->Horizontal = Horizontal;
		this->Start = Start;
		this->End = End;
		this->Depth = Depth;
	}
};

struct FENConnectionDrawingPolicyFactory : public FGraphPanelPinConnectionFactory
{
	virtual ~FENConnectionDrawingPolicyFactory()
	{
	}

	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};

class FENConnectionDrawingPolicy : public FKismetConnectionDrawingPolicy
{
public:
	FENConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj, bool IsTree = false)
		: FKismetConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj), IsTree(IsTree)
	{
	}

	virtual void DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params) override;

	void ENComputeClosestPoint(const FVector2f& Start, const FVector2f& End);
	void ENComputeClosestPointDefault(const FVector2f& Start, const FVector2f& StartTangent, const FVector2f& End, const FVector2f& EndTangent);
	void ENDrawBubbles(const FVector2f& Start, const FVector2f& StartTangent, const FVector2f& End, const FVector2f& EndTangent);
	void ENDrawArrow(const FVector2f& Start, const FVector2f& End);

	void DrawDebugPoint(const FVector2f& Position, FLinearColor Color);

private:
	const UElectronicNodesSettings& ElectronicNodesSettings = *GetDefault<UElectronicNodesSettings>();
	bool ReversePins;
	float MinXOffset;
	float ClosestDistanceSquared;
	FVector2f ClosestPoint;
	TArray<ENRibbonConnection> RibbonConnections;
	TMap<FVector2f, int> PinsOffset;

	bool IsTree = false;

	void ENCorrectZoomDisplacement(FVector2f& Start, FVector2f& End) const;
	void ENProcessRibbon(int32 LayerId, FVector2f& Start, FVector2f& StartDirection, FVector2f& End, FVector2f& EndDirection, const FConnectionParams& Params);
	bool ENIsRightPriority(const FConnectionParams& Params);
	int32 ENGetZoomLevel();
	int8 ENGetPinMembersCount(const UEdGraphPin* Pin);
	void ENDrawMainWire(FENPathDrawer* PathDrawer, EWireStyle WireStyle, FVector2f& Start, FVector2f& StartDirection, FVector2f& End, FVector2f& EndDirection, const FConnectionParams& Params);

	TSharedPtr<SGraphPanel> GetGraphPanel();
	void BuildRelatedNodes(UEdGraphNode* Node, TArray<UEdGraphNode*>& RelatedNodes, bool InputCheck, bool OutputCheck);

	int32 _LayerId;
	const FConnectionParams* _Params;
};
