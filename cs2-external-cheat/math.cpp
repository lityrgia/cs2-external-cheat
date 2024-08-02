#include "DataTypes.h"
#include <cmath>
#include "offsets.h"

bool WorldToScreen(const Vec3& VecOrigin, Vec2& VecScreen, float* Matrix) {
	float W = VecOrigin.x * Matrix[12] + VecOrigin.y * Matrix[13] + VecOrigin.z * Matrix[14] + Matrix[15];

	if (W < 0.01f)
		return false;

	Vec2 clipCoords;
	clipCoords.x = VecOrigin.x * Matrix[0] + VecOrigin.y * Matrix[1] + Matrix[3];
	clipCoords.y = VecOrigin.x * Matrix[4] + VecOrigin.y * Matrix[5] + VecOrigin.z * Matrix[6] + Matrix[7];

	Vec2 NDC;   
	NDC.x = clipCoords.x / W;
	NDC.y = clipCoords.y / W;

	VecScreen.x = (offsets.windowWidth / 2 * NDC.x) + (NDC.x + offsets.windowWidth / 2);
	VecScreen.y = -(offsets.windowHeight / 2 * NDC.y) + (NDC.y + offsets.windowHeight / 2);

	return true;
}

float getDistanceToEnemy3D(const Vec3& absCoords) {
	return sqrt(absCoords.x * absCoords.x + absCoords.y * absCoords.y + absCoords.z * absCoords.z);
}

float getDistanceToCenter(const Vec2& screenPos) {
	return sqrt(
		(offsets.windowWidth / 2 - screenPos.x) * (offsets.windowWidth / 2 - screenPos.x) +
		(offsets.windowHeight / 2 - screenPos.y) * (offsets.windowHeight / 2 - screenPos.y)
	);
}