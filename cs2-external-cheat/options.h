#pragma once
#include "imgui/imgui.h"

namespace menu {
	inline bool bMenuVisible{ true };

	inline bool bEnableAim{ false };
	inline bool bFOV{ false };
	inline bool bSmooth{ false };
	inline bool bEnableTriggerbot{ false };

	inline bool bEnableLines{ false };
	inline bool bEnableESP{ false };
	inline bool bEnableBox2D{ false };
	inline bool bEnableBox2DFill{ false };
	inline bool bEnableBox2DTeam{ false };
	inline bool bEnableBox2DLines{ true };
	inline bool bEnableBox3D{ false };
	inline bool bEnableBox3DAnim{ false };
	inline bool bEnableHealth{ false };
	inline bool bEnableSkeletonTeam{ false };
	inline bool bEnableDirection{ false };
	inline bool bEnableBox3DTeam{ false };
	inline bool bEnableName{ false };
	inline bool bEnableJoints{ false };
	inline bool bEnableSkeleton{ false };
	inline bool bTeamVisible{ false };

	inline unsigned short bToggleButton{ 0x2d };
	inline unsigned short bEndButton{0x23};
}

namespace options {
	inline ImVec4 boxes2DFillColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	inline ImVec4 boxes2DLinesColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	inline ImVec4 boxes2DFillColorTeam = ImVec4(0.0f, 0.0f, 1.0f, 0.4f);
	inline ImVec4 boxes2DLinesColorTeam = ImVec4(0.0f, 0.0f, 1.0f, 0.4f);

	inline ImVec4 snapLinesColorTeam = ImVec4(0.f, 0.f, 1.f, 1.f);

	inline ImVec4 boxes3DLinesColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	inline ImVec4 boxes3DLinesColorTeam = ImVec4(0.0f, 0.0f, 1.0f, 0.4f);

	inline ImVec4 skeletonColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	inline ImVec4 skeletonColorTeam = ImVec4(0.0f, 0.0f, 1.0f, 0.4f);

	inline ImVec4 snapLinesColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	inline ImVec4 directionColor = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);

	inline float radiusJoints = 40.f;
	inline float smoothness = 1.0f;
	inline float radiusFOV = 40.f;
	inline int triggerbotDelay = 1;
	inline int aimingType = 0;
	inline int aimButton{ 0 };
	inline float box2DRounding = { 2.f };
}