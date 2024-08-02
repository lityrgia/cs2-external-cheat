#include "datatypes.h"

Vec3 operator- (const Vec3& first, const Vec3& second) {
	Vec3 temp;
	temp.x = first.x - second.x;
	temp.y = first.y - second.y;
	temp.z = first.z - second.z;
	return temp;
};

Vec2 operator- (const Vec2& first, const Vec2& second) {
	Vec2 temp;
	temp.x = first.x - second.x;
	temp.y = first.y - second.y;
	return temp;
};

Vec2 operator/ (const Vec2& first, const float& second) {
	Vec2 temp;
	temp.x = first.x / second;
	temp.y = first.y / second;
	return temp;
};