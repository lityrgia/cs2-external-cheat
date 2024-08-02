#include <iostream>
#include <Windows.h>
#include "datatypes.h"
#include "cheat.h"
#include "options.h"
#include "visuals.h"
#include "offsets.h"
#include "memory.h"
#include "math.h"
#include "aimbot.h"

void startCheat() {
	game::CSplayerController localPlayerController;
	localPlayerController.controller = mem.Read<uintptr_t>(offsets.localController);

	game::CSplayerPawn localPlayerPawn;
	localPlayerPawn.pawn = mem.Read<uintptr_t>(offsets.localPawn);
	localPlayerPawn.getTeamNum();
	localPlayerPawn.getViewAngles();
	localPlayerPawn.getFeetPos();
	localPlayerPawn.getHeadPos();
	localPlayerPawn.getIndex();

	Matrix viewMatrix = mem.Read<Matrix>(offsets.viewMatrix);

	Vec2 targetScreenPosFin{0,0};

	int closestToCrosshair{ 999999 };
	float fov{ 300 };

	if (menu::bFOV) {
		drawFovCircle(options::radiusFOV);
	}

	offsets.getListEntry();

	for (short i = 1; i < 32; ++i) {
		game::CSplayerController entityController;
		game::CSplayerPawn entityPawn;

		entityController.num = i;
		entityController.getController();

		if (entityController.controller == 0 || entityController.controller == localPlayerController.controller) continue;

		entityPawn.value = entityController.get_CSPlayerPawn();
		entityPawn.getPawn();
		entityPawn.getPlayerHealth();

		if (entityPawn.playerHealth <= 0) continue;

		entityPawn.updateBones(viewMatrix);

		entityPawn.getTeamNum();
		entityPawn.getSpotted();
		entityPawn.getFeetPos();
		entityPawn.getHeadPos();
		entityPawn.getViewAngles();
		entityPawn.getIndex();
		entityController.getName();

		bool isTeammate = entityPawn.teamNum == localPlayerPawn.teamNum;

		if (!isTeammate && entityPawn.spotted != 0 && menu::bEnableAim) {
			Vec2 targetScreenPos;
			float distance{0};

			if (options::aimingType == 1) {
				 targetScreenPos = getClosestBoneScrPos(entityPawn.BonePosList);

				 distance = getDistanceToCenter(targetScreenPos);
				 
			}
			else if (options::aimingType == 0) {
				 targetScreenPos = entityPawn.BonePosList[head].ScreenPos;

				 distance = getDistanceToCenter(targetScreenPos);
			}
			
			if (distance < options::radiusFOV && distance < closestToCrosshair)
			{
				targetScreenPosFin = targetScreenPos;
				closestToCrosshair = distance;
			}
		}

		if (menu::bEnableESP) {
			startEsp(viewMatrix, entityPawn, entityController.name, isTeammate);
		}	
	}

	if (menu::bEnableTriggerbot) {
		startTriggerbot(localPlayerPawn);
	}

	int aimButton = options::aimButton;
	bool isAimingEnabled = menu::bEnableAim && targetScreenPosFin.x != 0 && targetScreenPosFin.y != 0;

	if (isAimingEnabled) {
		if (aimButton == 0) {
			startAimbot(localPlayerPawn, targetScreenPosFin);
		}
		else {
			int mouseButton = (aimButton == 1) ? VK_LBUTTON : VK_RBUTTON;

			if (GetAsyncKeyState(mouseButton) & 0x8000) {
				startAimbot(localPlayerPawn, targetScreenPosFin);
			}
		}
	}
}