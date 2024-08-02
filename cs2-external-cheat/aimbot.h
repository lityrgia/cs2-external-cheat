#pragma once

void startAimbot(game::CSplayerPawn& localPlayerPawn, Vec2& targetScreenPos);
void drawFovCircle(float radius);
void startTriggerbot(game::CSplayerPawn& localPlayer);
Vec2 getClosestBoneScrPos(std::vector<BoneJointPos>& bones);