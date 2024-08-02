#include "offsets.h"
#include <iostream>
#include <chrono>
#include "math.h"
#include "memory.h"
#include <Vector>
#include "datatypes.h"
#include <fstream>
#include <cstddef>
#include "json.h"
#include "config.h"

using json = nlohmann::json;

json clientJson;

bool game::baseOffsets::initialize() {
	printf("[cheat]: Waiting for cs2.exe...\n");

	while (true) {
		mem.attachProcess(L"cs2.exe");

		if (mem.processId == 0) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			continue;
		}
		else {
			printf("[cheat]: Attached cs2.exe\n");
			break;
		}
	}

	json offsetsJson;

	std::ifstream offsetsFile(config.offsetsPath + "\\offsetsDump.json");
	std::ifstream clientFile(config.offsetsPath + "\\clientDump.json");

	if (offsetsFile.is_open() && clientFile.is_open()) {
		offsetsFile >> offsetsJson;
		clientFile >> clientJson;

		offsetsFile.close();
		clientFile.close();
	}

	uintptr_t clientDll = mem.GetModuleAddress(L"client.dll");
	uintptr_t engineDll = mem.GetModuleAddress(L"engine2.dll");

	if (clientDll == 0 || engineDll == 0) {
		printf("[cheat]: Unable to read client.dll or engine.dll\n");
		return false;
	}

	this->clientDll = clientDll;
	this->engineDll = engineDll;

	uintptr_t temp = 0;

	temp = mem.Read<uintptr_t>(offsetsJson["engine2.dll"]["dwWindowWidth"] + engineDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read window width\n");
		return false;
	}

	this->windowWidth = temp;

	temp = mem.Read<uintptr_t>(offsetsJson["engine2.dll"]["dwWindowHeight"] + engineDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read window height\n");
		return false;
	}

	this->windowHeight = temp;

	temp = mem.Read<uintptr_t>(offsetsJson["client.dll"]["dwEntityList"] + clientDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read entity list\n");
		return false;
	}

	this->entityList = temp;

	temp = mem.Read<uintptr_t>(offsetsJson["client.dll"]["dwLocalPlayerController"] + clientDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read local player\n");
		return false;
	}
	temp = offsetsJson["client.dll"]["dwLocalPlayerController"] + clientDll;

	this->localController = temp;

	temp = mem.Read<uintptr_t>(offsetsJson["client.dll"]["dwViewMatrix"] + clientDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read view matrix\n");
		return false;
	}

	temp = offsetsJson["client.dll"]["dwViewMatrix"] + clientDll;

	this->viewMatrix = temp;

	while (true) {
		temp = mem.Read<uintptr_t>(offsetsJson["client.dll"]["dwViewAngles"] + clientDll);
		if (temp == 0) {
			printf("[cheat]: Unable to read view angles, start game\n");
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		else {
			break;
		}
	}

	temp = offsetsJson["client.dll"]["dwViewAngles"] + clientDll;

	this->viewAngle = temp;

	temp = mem.Read<uintptr_t>(offsetsJson["client.dll"]["dwLocalPlayerPawn"] + clientDll);
	if (temp == 0) {
		printf("[cheat]: Unable to read local pawn\n");
		return false;
	}
	temp = offsetsJson["client.dll"]["dwLocalPlayerPawn"] + clientDll;

	this->localPawn = temp;

	return true;
}

uintptr_t game::baseOffsets::getListEntry() {
	this->listEntry = mem.Read<uintptr_t>(offsets.entityList + 0x10);
	return listEntry;
}

int game::CSplayerPawn::getSpotted() {
	this->spotted = mem.Read<int>(this->pawn + clientJson["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]["m_entitySpottedState"] + clientJson["client.dll"]["classes"]["EntitySpottedState_t"]["fields"]["m_bSpottedByMask"]);
	return spotted;
}

uint8_t game::CSplayerPawn::getTeamNum() {
	this->teamNum = mem.Read<unsigned short>(this->pawn + clientJson["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iTeamNum"]);
	return teamNum;
}

bool game::CSplayerController::getIsAlive() {
	this->isPawnAlive = mem.Read<bool>(this->controller + clientJson["client.dll"]["classes"]["CCSPlayerController"]["fields"]["m_bPawnIsAlive"]);
	return isPawnAlive;
}

uintptr_t game::CSplayerPawn::getPawn() {
	this->pawn = mem.Read<uintptr_t>(offsets.listEntry + 0x78 * (this->value & 0x1FF));
	return pawn;
}

uintptr_t game::CSplayerController::getController() {
	this->controller = mem.Read<uintptr_t>(offsets.listEntry + 0x78 * (uintptr_t)(this->num & 0x1FF));
	return controller;
}

uintptr_t game::CSplayerController::get_CSPlayerPawn() {
	return mem.Read<uintptr_t>(this->controller + clientJson["client.dll"]["classes"]["CCSPlayerController"]["fields"]["m_hPlayerPawn"]);
}

int game::CSplayerPawn::getPlayerHealth() {
	this->playerHealth = mem.Read<int>(this->pawn + clientJson["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iHealth"]);
	return playerHealth;
}

Vec3 game::CSplayerPawn::getFeetPos() {
	this->feetPos = mem.Read<Vec3>(this->pawn + clientJson["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]["m_vOldOrigin"]);
	return feetPos; 
}

Vec2 game::CSplayerPawn::getViewAngles() {
	this->viewAngles = mem.Read<Vec2>(this->pawn + clientJson["client.dll"]["classes"]["C_CSPlayerPawnBase"]["fields"]["m_angEyeAngles"]);
	return viewAngles;
}

Vec3 game::CSplayerPawn::getHeadPos() {
	this->headPos.x = this->feetPos.x;
	this->headPos.y = this->feetPos.y;
	this->headPos.z = this->feetPos.z + 72.f;

	return headPos;
}

int game::CSplayerPawn::getIndex(){
	this->crossHairID = mem.Read<int>(this->pawn + clientJson["client.dll"]["classes"]["C_CSPlayerPawnBase"]["fields"]["m_iIDEntIndex"]);
    return crossHairID;
}

uintptr_t game::CSplayerPawn::getPlayerPawnByCrossHairID(int crossHairEntity) {
	uintptr_t crosshairEntityEntry = mem.Read<uintptr_t>(this->value + 0x8 * (crossHairEntity >> 9) + 0x10);
	this->pawn = mem.Read<uintptr_t>(crosshairEntityEntry + 0x78 * (crossHairEntity & 0x1FF));
	return pawn;
}

std::string game::CSplayerController::getName() {
	uintptr_t temp = mem.Read<uintptr_t>(controller + clientJson["client.dll"]["classes"]["CCSPlayerController"]["fields"]["m_sSanitizedPlayerName"]);
	if (temp) {
		char buff[50]{};
		mem.ReadRaw(temp, buff, 50);
		this->name = std::string(buff);
	}
	else {
		this->name = "unknown";
	}
	return name;
}

bool game::CSplayerPawn::updateBones(Matrix& viewMatrix) {

	uintptr_t GameSceneNode = mem.Read<uintptr_t>(this->pawn + clientJson["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_pGameSceneNode"]);

	if (GameSceneNode == 0) return false;

	uintptr_t boneMatrix = mem.Read<uintptr_t>(GameSceneNode + clientJson["client.dll"]["classes"]["CSkeletonInstance"]["fields"]["m_modelState"] + 0x80);

	if (boneMatrix == 0) return false;

	BoneJointData BoneArray[30]{};

	mem.ReadRaw(boneMatrix, reinterpret_cast<char*>(BoneArray), 30 * sizeof(BoneJointData));

	for (int i = 0; i < 30; i++)
	{
		Vec2 ScreenPos;
		bool IsVisible = false;

		if (WorldToScreen(BoneArray[i].Pos, ScreenPos, viewMatrix.VMatrix))
			IsVisible = true;

		this->BonePosList.push_back({ BoneArray[i].Pos ,ScreenPos,IsVisible });
	}

	return this->BonePosList.size() > 0;
}