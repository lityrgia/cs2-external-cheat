#pragma once
#include "imgui/imgui.h"
#include "datatypes.h" 
#include "offsets.h"

void startEsp(Matrix& viewMatrix, game::CSplayerPawn& playerPawn, std::string& entityName, bool teammate);
void drawLines(Vec2& end, ImColor color);
void drawBox2D(Vec2& coordsHead, Vec2& coordsFeet, ImColor colorFill, ImColor colorLines);
void drawBox3D(Matrix& viewMatrix, Vec3& playerLocationFeet, Vec3& playerLocationHead, ImColor colorLines);
void drawHealth(Vec2& coordsHead, Vec2& coordsFeet, short entityHealth);
void drawViewDirection(Vec2& coordsHead, Vec2& entityViewAngles, Vec3& entityLocationHead, Matrix& viewMatrix, ImColor color);
void drawName(Vec2& coordsHead, std::string& entityName);
void drawSkeleton(game::CSplayerPawn& entityPawn, ImColor colorSkeleton);