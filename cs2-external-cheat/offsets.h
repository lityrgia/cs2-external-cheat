#pragma once
#include <string>
#include <Windows.h>
#include <memory>
#include <vector>

#include "datatypes.h"
#include <list>

namespace game {
	class baseOffsets {
	public:
		int windowHeight;
		int windowWidth;

		uintptr_t clientDll;
		uintptr_t engineDll;

		uintptr_t entityList;
		uintptr_t viewMatrix;
		uintptr_t viewAngle;
		uintptr_t entityListEntry;
		uintptr_t localController;
		uintptr_t localPawn;

		uintptr_t listEntry;
	public:
		//bool update();
		bool initialize();
		uintptr_t getListEntry();
	};

	class CSplayerController {
	public:
		int num;
		uintptr_t controller{ 0 };
		std::string name;

		bool isPawnAlive{ true };

		bool getIsAlive();

		uintptr_t getController();
		uintptr_t get_CSPlayerPawn();

		std::string getName();
	};

	class CSplayerPawn {
	public:
		uintptr_t value, pawn;
		uintptr_t getPawn();

		int playerHealth;
		int getPlayerHealth();

		Vec3 feetPos, headPos;
		Vec3 getFeetPos();
		Vec3 getHeadPos();

		Vec2 headPosScreen;

		Vec2 viewAngles;
		Vec2 getViewAngles();

		uint8_t teamNum{};
		uint8_t getTeamNum();

		int crossHairID;
		int getIndex();

		int spotted;
		int getSpotted();

		uintptr_t getPlayerPawnByCrossHairID(int crossHairEntity);

		std::vector<BoneJointPos> BonePosList;

		bool updateBones(Matrix& viewMatrix );
	};
}

inline game::baseOffsets offsets;

enum BONEINDEX : DWORD
{
	head = 6,
	neck_0 = 5,
	spine_1 = 4,
	spine_2 = 2,
	pelvis = 0,
	arm_upper_L = 8,
	arm_lower_L = 9,
	hand_L = 10,
	arm_upper_R = 13,
	arm_lower_R = 14,
	hand_R = 15,
	leg_upper_L = 22,
	leg_lower_L = 23,
	ankle_L = 24,
	leg_upper_R = 25,
	leg_lower_R = 26,
	ankle_R = 27,
};

namespace BoneJointList
{
	// ¼¹¹Ç
	inline std::list<DWORD> Trunk = { head,neck_0,spine_2, pelvis };
	// ×ó±Û
	inline std::list<DWORD> LeftArm = { neck_0,  arm_upper_L, arm_lower_L, hand_L };
	// ÓÒ±Û
	inline std::list<DWORD> RightArm = { neck_0, arm_upper_R,arm_lower_R, hand_R };
	// ×óÍÈ	
	inline std::list<DWORD> LeftLeg = { pelvis, leg_upper_L , leg_lower_L, ankle_L };
	// ÓÒÍÈ
	inline std::list<DWORD> RightLeg = { pelvis, leg_upper_R , leg_lower_R, ankle_R };
	// ×ÜÁÐ±í
	inline std::vector<std::list<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}