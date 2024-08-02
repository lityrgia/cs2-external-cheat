#pragma once

#include "DataTypes.h"

bool WorldToScreen(const Vec3& VecOrigin, Vec2& VecScreen, float* Matrix);
float getDistanceToEnemy3D(const Vec3& absCoords);
float getDistanceToCenter(const Vec2& screenPos);