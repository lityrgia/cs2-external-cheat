#pragma once
#include "imgui.h"

struct Vec2 {
	float x;
	float y;

	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float x, float y) : x(x), y(y) {}
	Vec2(const ImVec2& imVec) : x(imVec.x), y(imVec.y) {}

	operator ImVec2() const { return ImVec2(x, y); }

	bool operator< (const Vec2& other) {
		if (x < other.x) return true;
		if (x > other.x) return false;
		return y < other.y;
	}
};

struct Vec3 {
	float x;
	float y;
	float z;
};

Vec3 operator- (const Vec3& first, const Vec3& second);
Vec2 operator- (const Vec2& first, const Vec2& second);
Vec2 operator/ (const Vec2& first, const float& second);

struct Vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct BoneJointData
{
	Vec3 Pos;
	char pad[0x14];
};

struct BoneJointPos
{
	Vec3 Pos;
	Vec2 ScreenPos;
	bool IsVisible = false;
};

struct Matrix {
	float VMatrix[16];
};