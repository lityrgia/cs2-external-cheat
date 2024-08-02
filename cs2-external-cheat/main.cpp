#include <iostream>
#include "overlay.h"
#include "offsets.h"
#include "imgui.h"
#include "options.h"
#include "gui.h"
#include "cheat.h"
#include "overlay.h"
#include "config.h"


int main() {
    system("title Vireless output");

    if (config.initialize()) {
        printf("[cheat]: Succesfly initialized configs\n");
    }
    else {
        printf("[cheat]: Failed to initialize configs\n");
    }

    if (config.downloadOffsetsFromGithub()) {
        std::cout << "[cheat]: Succesfly downloaded offsets" << '\n';
    }
    else {
        return 0;
    }

    if (offsets.initialize()) {
        printf("[cheat]: Succesfly initialized offsets\n");
    }
    else {
        printf("[cheat]: Failed to initialize offsets\n");
        return 0;
    }   

    if (overlay.CreateOverlay()) {
        printf("[cheat]: Succesfly created overlay\n");
    }
    else {
        printf("[cheat]: Failed to create overlay\n");
        return 0;
    }

    ImGui::CreateContext();
    SetupFonts();
    setImguiStyle();
    overlay.imguiInit();

    printf("[cheat]: Starting main loop\n\n");

    MSG msg;
    while (overlay.done == false)
    {
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                overlay.done = true;
                break;
            }
        }

        if (overlay.done == true || GetAsyncKeyState(menu::bEndButton) & 1) break;

        if (GetAsyncKeyState(menu::bToggleButton) & 1)  menu::bMenuVisible = !menu::bMenuVisible;

        overlay.MakeNewFrame();

        if (menu::bMenuVisible)
        {
            guiStart();
            SetWindowLong(overlay.hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
        }
        else
        {
            SetWindowLong(overlay.hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        }

        startCheat();
        overlay.RenderGui();
    }

    overlay.ReleaseOverlay();

	return 1;
}
