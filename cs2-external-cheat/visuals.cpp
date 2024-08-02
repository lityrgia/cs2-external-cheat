#include "imgui.h"
#include <iostream>
#include <string>
#include <format>

#include "visuals.h"
#include "math.h"
#include "options.h"
#include "memory.h"
#include "datatypes.h"
#include "offsets.h"
#include "visuals.h"

#define M_PI 3.14159265358979323846

ImColor colorHealth(50, 205, 50, 255);

void startEsp(Matrix& viewMatrix, game::CSplayerPawn& playerPawn, std::string& entityName, bool teammate) {

	Vec2 screenFeetCoords;
	Vec2 screenHeadCoords;

	if (!WorldToScreen(playerPawn.headPos, screenHeadCoords, viewMatrix.VMatrix)) return;
	if (!WorldToScreen(playerPawn.feetPos, screenFeetCoords, viewMatrix.VMatrix)) return;

	ImU32 lineColor;
	ImU32 boxFillColor;
	ImU32 boxLineColor;
	ImU32 box3DLineColor;
	ImU32 skeletonColor;

	if (teammate) {
		if (menu::bTeamVisible) {
			lineColor = ImColor(options::snapLinesColorTeam);
			boxFillColor = ImColor(options::boxes2DFillColorTeam);
			boxLineColor = ImColor(options::boxes2DLinesColorTeam);
			box3DLineColor = ImColor(options::boxes3DLinesColorTeam);
			skeletonColor = ImColor(options::skeletonColorTeam);
		}
		else {
			boxFillColor = ImColor(0.f, 0.f, 0.f, 0.f); 
			lineColor = ImColor(0.f, 0.f, 0.f, 0.f);
			boxLineColor = ImColor(0.f, 0.f, 0.f, 0.f);
			box3DLineColor = ImColor(0.f, 0.f, 0.f, 0.f);
			skeletonColor = ImColor(0.f, 0.f, 0.f, 0.f);
		}
	}
	else {
		lineColor = ImColor(options::snapLinesColor);
		boxFillColor = ImColor(options::boxes2DFillColor);
		boxLineColor = ImColor(options::boxes2DLinesColor);
		box3DLineColor = ImColor(options::boxes3DLinesColor);
		skeletonColor = ImColor(options::skeletonColor);
	}

	if (menu::bEnableLines) {
		drawLines(screenFeetCoords, lineColor);
	}
	if (menu::bEnableBox2D) {
		drawBox2D(screenHeadCoords, screenFeetCoords, boxFillColor, boxLineColor);
	}
	if (menu::bEnableBox3D) {
		drawBox3D(viewMatrix, playerPawn.feetPos, playerPawn.headPos, box3DLineColor);
	}
	if (menu::bEnableSkeleton) {
		drawSkeleton(playerPawn, skeletonColor);
	}

	if (teammate && !menu::bTeamVisible) return;

	if (menu::bEnableHealth) {
		drawHealth(screenHeadCoords, screenFeetCoords, playerPawn.playerHealth);
	}
	if (menu::bEnableDirection) {
		drawViewDirection(playerPawn.BonePosList[head].ScreenPos, playerPawn.viewAngles, playerPawn.headPos, viewMatrix, options::directionColor);
	}
	if (menu::bEnableName) {
		drawName(screenHeadCoords, entityName);
	}
}

void drawLines(Vec2& end, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(offsets.windowWidth / 2, offsets.windowHeight), end, color, 2.f);
}

void drawBox2D(Vec2& coordsHead, Vec2& coordsFeet, ImColor colorFill, ImColor colorLines) {
	float boxHeight = coordsFeet.y - coordsHead.y;
	float boxWidth = boxHeight / 4.5f;
	
	ImVec2 startPos = { coordsHead.x - boxWidth, coordsHead.y };
	ImVec2 endPos = { coordsHead.x + boxWidth, coordsFeet.y };

	if (menu::bEnableBox2DFill) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(startPos, endPos, colorFill, options::box2DRounding);
	}

	if (menu::bEnableBox2DLines) {
		ImGui::GetBackgroundDrawList()->AddRect(startPos, endPos, colorLines, options::box2DRounding, 0, 1.4f);
	}
}

void drawBox3D(Matrix& viewMatrix, Vec3& entityLocationFeet, Vec3& entityLocationHead, ImColor colorLines) {
	float h = entityLocationHead.z - entityLocationFeet.z;
	float w = h / 3.5f;

	float cx = (entityLocationHead.x + entityLocationFeet.x) / 2.0f;
	float cy = (entityLocationHead.y + entityLocationFeet.y) / 2.0f;
	float cz = (entityLocationHead.z + entityLocationFeet.z) / 2.0f;

	Vec3 vertices[8] = {
		{cx - w / 2, cy - w / 2, cz - h / 2},
		{cx + w / 2, cy - w / 2, cz - h / 2},
		{cx + w / 2, cy + w / 2, cz - h / 2},
		{cx - w / 2, cy + w / 2, cz - h / 2},
		{cx - w / 2, cy - w / 2, cz + h / 2},
		{cx + w / 2, cy - w / 2, cz + h / 2},
		{cx + w / 2, cy + w / 2, cz + h / 2},
		{cx - w / 2, cy + w / 2, cz + h / 2}
	};

	Vec2 screenCoord3DParts[8];

	for (int i = 0; i < 8; ++i) {
		WorldToScreen(vertices[i], screenCoord3DParts[i], viewMatrix.VMatrix);
	}

	Vec2 tempLines[4] = {
		screenCoord3DParts[4] - screenCoord3DParts[0],
		screenCoord3DParts[5] - screenCoord3DParts[1],
		screenCoord3DParts[6] - screenCoord3DParts[2],
		screenCoord3DParts[7] - screenCoord3DParts[3],
	};

	//animation lines

	static float counter = 0;
	static float speed = 0.01;

	counter += speed;

	float sineValue = (sin(counter) + 1) / 2;
	sineValue = sineValue * sineValue * (10 - 2 * sineValue);
	sineValue = sineValue * 8 + 1;

	//down

	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[0], screenCoord3DParts[1], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[1], screenCoord3DParts[2], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[2], screenCoord3DParts[3], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[3], screenCoord3DParts[0], colorLines, 2.5f);

	////up

	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[4], screenCoord3DParts[5], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[5], screenCoord3DParts[6], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[6], screenCoord3DParts[7], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[7], screenCoord3DParts[4], colorLines, 2.5f);

	////tempLines
 
	if (menu::bEnableBox3DAnim) {
		ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[4] - tempLines[0] / sineValue, screenCoord3DParts[5] - tempLines[1] / sineValue, colorLines, 2.5f);
		ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[5] - tempLines[1] / sineValue, screenCoord3DParts[6] - tempLines[2] / sineValue, colorLines, 2.5f);
		ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[6] - tempLines[2] / sineValue, screenCoord3DParts[7] - tempLines[3] / sineValue, colorLines, 2.5f);
		ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[7] - tempLines[3] / sineValue, screenCoord3DParts[4] - tempLines[0] / sineValue, colorLines, 2.5f);
	}

	////central

	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[0], screenCoord3DParts[4], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[1], screenCoord3DParts[5], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[2], screenCoord3DParts[6], colorLines, 2.5f);
	ImGui::GetBackgroundDrawList()->AddLine(screenCoord3DParts[3], screenCoord3DParts[7], colorLines, 2.5f);
}

void drawViewDirection(Vec2& coordsHead, Vec2& entityViewAngles, Vec3& entityLocationHead, Matrix& viewMatrix, ImColor color) {
	float viewDistance = 40.0f;

	float LineLength = cos(entityViewAngles.x * M_PI / 180) * viewDistance;

	Vec3 endPoint;
	endPoint.x = entityLocationHead.x + cos(entityViewAngles.y * M_PI / 180) * LineLength;
	endPoint.y = entityLocationHead.y + sin(entityViewAngles.y * M_PI / 180) * LineLength;
	endPoint.z = entityLocationHead.z - sin(entityViewAngles.x * M_PI / 180) * viewDistance;

	Vec2 screenCoordEnd;
	WorldToScreen(endPoint, screenCoordEnd, viewMatrix.VMatrix);

	ImGui::GetBackgroundDrawList()->AddLine(coordsHead, screenCoordEnd, color, 2.0f);
}

void drawHealth(Vec2& coordsHead, Vec2& coordsFeet, short entityHealth) {
	float boxHeight = coordsHead.y - coordsFeet.y;

	float boxWidth = boxHeight / 4.4f;
	float boxWidth2 = boxWidth * 1.07f;

	float healthBarHeight = boxHeight * (entityHealth / 100.0f);
	float healthBarTopY = coordsFeet.y + healthBarHeight;

	ImVec2 start{ coordsHead.x + boxWidth, coordsFeet.y};
	ImVec2 end{ coordsHead.x + boxWidth2, healthBarTopY };

	ImGui::GetBackgroundDrawList()->AddRectFilled(start, end, colorHealth, 0.f);
}

void drawName(Vec2& coordsHead, std::string& entityName) {
	ImFont* font = ImGui::GetFont();
	float fontSize = ImGui::GetFontSize();	
	     
	ImVec2 textSize = ImGui::CalcTextSize(entityName.c_str(), nullptr, false, fontSize);

	ImVec2 centeredPos = ImVec2(coordsHead.x - textSize.x / 2.f, coordsHead.y - textSize.y / 3.f);

	ImGui::GetBackgroundDrawList()->AddText(font, fontSize, centeredPos, ImColor(255, 255, 255, 255), entityName.c_str());
}

void drawSkeleton(game::CSplayerPawn& entityPawn, ImColor colorSkeleton) {
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[head].ScreenPos, entityPawn.BonePosList[neck_0].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[neck_0].ScreenPos, entityPawn.BonePosList[arm_upper_L].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[neck_0].ScreenPos, entityPawn.BonePosList[arm_upper_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[neck_0].ScreenPos, entityPawn.BonePosList[spine_1].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[spine_1].ScreenPos, entityPawn.BonePosList[spine_2].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[arm_lower_L].ScreenPos, entityPawn.BonePosList[arm_upper_L].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[arm_lower_R].ScreenPos, entityPawn.BonePosList[arm_upper_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[spine_2].ScreenPos, entityPawn.BonePosList[pelvis].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[pelvis].ScreenPos, entityPawn.BonePosList[leg_upper_L ].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[pelvis].ScreenPos, entityPawn.BonePosList[leg_upper_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[arm_lower_L].ScreenPos, entityPawn.BonePosList[hand_L].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[arm_lower_R].ScreenPos, entityPawn.BonePosList[hand_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[leg_upper_L].ScreenPos, entityPawn.BonePosList[leg_lower_L].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[leg_upper_R].ScreenPos, entityPawn.BonePosList[leg_lower_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[leg_lower_R].ScreenPos, entityPawn.BonePosList[ankle_R].ScreenPos, colorSkeleton);
	ImGui::GetBackgroundDrawList()->AddLine(entityPawn.BonePosList[leg_lower_L].ScreenPos, entityPawn.BonePosList[ankle_L].ScreenPos, colorSkeleton);
}