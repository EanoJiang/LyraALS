/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"
#include "ENConnectionDrawingPolicy.h"
#include "../Public/ElectronicNodesSettings.h"

class FENPathDrawer
{
public:
	FENPathDrawer(int32& LayerId, float& ZoomFactor, bool RightPriority, const FConnectionParams* Params, FSlateWindowElementList* DrawElementsList, FENConnectionDrawingPolicy* ConnectionDrawingPolicy);

	void DrawManhattanWire(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);
	void DrawSubwayWire(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);
	void DrawDefaultWire(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);

	void DrawIntersectionRadius(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);
	void DrawIntersectionDiagRadius(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);

	void DrawSimpleRadius(const FVector2f& Start, const FVector2f& StartDirection, const int32& AngleDeg, FVector2f& out_End, FVector2f& out_EndDirection, bool Backward = false);
	void DrawUTurn(const FVector2f& Start, const FVector2f& StartDirection, float Direction, FVector2f& out_End, FVector2f& out_EndDirection, bool Backward = false);
	void DrawCorrectionOrtho(const FVector2f& Start, const FVector2f& StartDirection, const float& Displacement, FVector2f& out_End, FVector2f& out_EndDirection, bool Backward = false);

	float GetRadiusOffset(const int32& AngleDeg = 0, bool Perpendicular = false);
	float GetRadiusTangent(const int32& AngleDeg = 0);
	float GetIntersectionOffset(const int32& AngleDeg = 0, bool Diagonal = false);

	void DrawOffset(FVector2f& Start, FVector2f& StartDirection, const float& Offset, bool Backward = false);
	void DrawLine(const FVector2f& Start, const FVector2f& End);
	void DrawRadius(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection, const int32& AngleDeg);
	void DrawSpline(const FVector2f& Start, const FVector2f& StartDirection, const FVector2f& End, const FVector2f& EndDirection);

	void DebugColor(const FLinearColor& Color);

private:
	const UElectronicNodesSettings& ElectronicNodesSettings = *GetDefault<UElectronicNodesSettings>();

	int32 LayerId;
	float ZoomFactor;
	bool RightPriority;

	FLinearColor WireColor;
	const FConnectionParams* Params;
	FSlateWindowElementList* DrawElementsList;
	FENConnectionDrawingPolicy* ConnectionDrawingPolicy;

	int32 MaxDepthWire = 5;
};
