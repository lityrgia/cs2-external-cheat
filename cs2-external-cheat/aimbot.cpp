#include <string>
#include <iostream>
#include <numbers>
#include <cmath>
#include <chrono>
#include <vector>

#include "Memory.h"
#include "datatypes.h"
#include "imgui.h"
#include "offsets.h"
#include "math.h"
#include "options.h"
#include "aimbot.h"

#define M_PI 3.14159265358979323846

void moveMouseByOffset(int offsetX, int offsetY) {
    // Получите текущее положение мыши
    POINT pt;
    if (GetCursorPos(&pt)) {
        int newX = pt.x + offsetX;
        int newY = pt.y + offsetY;

        mouse_event(MOUSEEVENTF_MOVE, offsetX, offsetY, 0, 0);
    }
}

void startAimbot(game::CSplayerPawn& localPlayerPawn, Vec2& targetScreenPos) {
    Vec2 screenCenter{ offsets.windowWidth / 2.f, offsets.windowHeight / 2.f };

    moveMouseByOffset(-(screenCenter.x - targetScreenPos.x) / options::smoothness, -(screenCenter.y - targetScreenPos.y) / options::smoothness);
}

void startTriggerbot(game::CSplayerPawn& localPlayer) {

    int crosshairID = 0;
    crosshairID = localPlayer.getIndex();

    if (crosshairID == -1) return;

    uintptr_t listEntry = 0;
    listEntry = mem.Read<uintptr_t>(offsets.entityList + 0x8 * (crosshairID >> 9) + 0x10);

    if (listEntry == 0) return;

    uintptr_t pawnAddress = 0;
    pawnAddress = mem.Read<uintptr_t>(listEntry + 0x78 * (crosshairID & 0x1FF));

    if (pawnAddress == 0) return;

    game::CSplayerPawn entityFound;
    entityFound.pawn = pawnAddress;
    entityFound.getTeamNum();
    entityFound.getPlayerHealth();

    if (entityFound.teamNum == localPlayer.teamNum || entityFound.playerHealth < 1) return;

    static std::chrono::time_point lastTimePoint = std::chrono::steady_clock::now();
    auto curTimePoint = std::chrono::steady_clock::now();
    if (curTimePoint - lastTimePoint >= std::chrono::milliseconds(options::triggerbotDelay))
    {
        const bool shooting = GetAsyncKeyState(VK_LBUTTON) < 0;
        if (!shooting)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }

        lastTimePoint = curTimePoint;
    }
}

Vec2 getClosestBoneScrPos(std::vector<BoneJointPos>& bones) {
    Vec2 closestBonePos{0,0};
    float minDistance{999999.f};

    for (const auto& bone : bones) {
        const Vec2& temp = bone.ScreenPos;
        float boneDistance = getDistanceToCenter(temp);
        if (minDistance > boneDistance) {
            minDistance = boneDistance;
            closestBonePos = temp;
        }
    }

    return closestBonePos;
}

void drawFovCircle(float radius) {
    ImGui::GetBackgroundDrawList()->AddCircle( ImVec2{offsets.windowWidth / 2.f, offsets.windowHeight / 2.f}, radius, ImColor(255, 255, 255, 255), 25, 2.f);
}