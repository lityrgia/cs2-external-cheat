#include "gui.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>

#include "imgui/imgui.h"

#include "config.h"
#include "options.h"
#include "fonts.hpp"

void setImguiStyle() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);

    colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ButtonActive] = ImColor(255, 122, 0, 255);
    colors[ImGuiCol_ButtonHovered] = ImColor(255, 140, 0, 255);

    colors[ImGuiCol_SliderGrab] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_SliderGrabActive] = ImColor(16, 24, 32, 255);

    colors[ImGuiCol_CheckMark] = ImColor(0, 0, 0, 255);

    colors[ImGuiCol_FrameBg] = ImColor(255, 255, 255);
    colors[ImGuiCol_FrameBgActive] = ImColor(255, 255, 255);
    colors[ImGuiCol_FrameBgHovered] = ImColor(255, 255, 255);

    colors[ImGuiCol_PopupBg] = ImColor(255, 255, 255, 255);

    colors[ImGuiCol_WindowBg] = ImColor(16, 24, 32, 255);

    colors[ImGuiCol_TitleBg] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_TitleBgActive] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_TitleBgCollapsed] = ImColor(16, 24, 32, 255);

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FramePadding = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    style.ItemSpacing = ImVec2(0, 0);
    style.WindowRounding = 5.f;
    style.FrameRounding = 4.0f;
    style.GrabMinSize = 9.f;
    style.GrabRounding = 100.f;
}

void DrawLine(ImColor color, float thickness, std::string direction) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 lineStart;
    ImVec2 lineEnd;

    if (direction == "up") {
        lineStart = ImVec2(windowPos.x, windowPos.y);
        lineEnd = ImVec2(windowPos.x + windowSize.x - 1, windowPos.y);
    }
    else if (direction == "down") {
        lineStart = ImVec2(windowPos.x, windowPos.y + windowSize.y);
        lineEnd = ImVec2(windowPos.x + windowSize.x - 1, windowPos.y + windowSize.y);
    }
    else if (direction == "right") {
        lineStart = ImVec2(windowPos.x + windowSize.x - 1, windowPos.y);
        lineEnd = ImVec2(windowPos.x + windowSize.x - 1, windowPos.y + windowSize.y);
    }
    else if (direction == "left") {
        lineStart = ImVec2(windowPos.x, windowPos.y);
        lineEnd = ImVec2(windowPos.x, windowPos.y + windowSize.y);
    }
    else {
        return;
    }

    drawList->AddLine(lineStart, lineEnd, color, thickness);
}

void ShowColorPickerButton(ImVec4* color, int id) {
    ImGui::PushID(id);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 1));

    if (ImGui::ColorButton("ColorButton", *color)) {
        ImGui::OpenPopup("ColorPickerPopup");
    }

    if (ImGui::BeginPopup("ColorPickerPopup")) {
        ImGui::ColorPicker4("##picker", (float*)color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void CenteredText(const char* text) {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f); 
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); 
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize(text);
    float x = (windowSize.x - textSize.x) * 0.5f;
    ImGui::SetCursorPosX(x);
    ImGui::Text("%s", text);

    ImGui::PopStyleVar(3); 
}

bool ToggleButton(const char* str_id, bool* v) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    ImU32 col_bg = *v ? IM_COL32(255, 140, 0, 255) : IM_COL32(255, 255, 255, 255);
    ImU32 circle_bg = *v ? IM_COL32(255, 255, 255, 255) : IM_COL32(16, 24, 32, 255);

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, circle_bg);

    return *v;
}

inline ImFont* iconsFont = nullptr;
inline ImFont* largeFont = nullptr;

ImVec2 menuSize(600, 500);
ImVec2 buttonSize(140, 40);
ImColor separatorsColor(255, 140, 0, 255);
ImVec2 paddings(0.f, 15.f);

int selectedConfigIndex = -1;
std::string selectedConfig;

bool snaplinessettings = false;
bool box2dsettings = false;
bool box3dsettings = false;
bool skeletonsettings = false;

char configName[256] = "";

static std::string keyName = "Press a key"; 
bool waitingForToggleKey = false;
bool waitingForEndKey = false;

bool keyPressed = false;

static int selectedTab = 0;  // 0: Aim, 1: ESP, 2: Config, 3: Settings

const char* aimingTypes[] = { "Head", "Nearest Bone" };
const char* aimbotButtons[] = { "Auto", "Left button", "Right button"};

void SetupFonts() {
    static const ImWchar iconRanges[]{ ICON_MIN_FA, ICON_MAX_FA, 0 };

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    static const ImWchar* glyphRanges = io.Fonts->GetGlyphRangesCyrillic();

    ImFont* fontLarge = io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 19.f, nullptr, glyphRanges);

    largeFont = io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 30.f);

    io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 19.f);

    ImFontConfig iconsConfig;

    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.OversampleH = 3;
    iconsConfig.OversampleV = 3;

    iconsFont = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 22.0f, &iconsConfig, iconRanges);
}

void guiStart() {

            ImGui::Begin("Vireless", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::SetWindowSize(menuSize, ImGuiCond_Always);
            
            ImGui::PushFont(largeFont);

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            CenteredText("Vireless");
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::PopFont();

            ImGui::BeginChild("LeftPane", ImVec2(160, 0), true);

            ImVec2 windowSize = ImGui::GetWindowSize();

            ImGui::PushFont(iconsFont);

            ImGui::NewLine();

            if (selectedTab == 0) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 140, 0, 255));
            ImGui::Dummy(ImVec2((windowSize.x - buttonSize.x) * 0.5f, 0));
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CROSSHAIRS "  Aimbot     ", buttonSize)) selectedTab = 0;
            if (selectedTab == 0) ImGui::PopStyleColor();

            ImGui::NewLine();

            if (selectedTab == 1) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 140, 0, 255));
            ImGui::Dummy(ImVec2((windowSize.x - buttonSize.x) * 0.5f, 0));
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_EYE "  Visuals     ", buttonSize)) selectedTab = 1;
            if (selectedTab == 1) ImGui::PopStyleColor();

            ImGui::NewLine();

            if (selectedTab == 2) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 140, 0, 255));
            ImGui::Dummy(ImVec2((windowSize.x - buttonSize.x) * 0.5f, 0));
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FOLDER "  Config     ", buttonSize)) selectedTab = 2;
            if (selectedTab == 2) ImGui::PopStyleColor();

            ImGui::NewLine();

            if (selectedTab == 3) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 140, 0, 255));
            ImGui::Dummy(ImVec2((windowSize.x - buttonSize.x) * 0.5f, 0));
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_BULLSEYE "  Settings  ", buttonSize)) selectedTab = 3;
            if (selectedTab == 3) ImGui::PopStyleColor();

            DrawLine(separatorsColor, 1.0f, "up");
            DrawLine(separatorsColor, 1.0f, "right");

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("RightPane", ImVec2(0, 0), true);

            DrawLine(separatorsColor, 1.0f, "up");
            ImGui::Indent(20.f);

            ImGui::NewLine();

            if (selectedTab == 0) {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

                ImGui::BeginChild("AimbotFrame", ImVec2(404, 280), true);

                ImGui::Indent(14.f);

                ImGui::Dummy(ImVec2(0, 15));
                ToggleButton("asdf", &menu::bEnableAim);
                ImGui::SameLine();
                ImGui::Text("  Enable aimbot");

                ImGui::Dummy(paddings);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::Combo("##comboAimingButton", &options::aimButton, aimbotButtons, IM_ARRAYSIZE(aimbotButtons));
                ImGui::SameLine();
                ImGui::PopStyleColor(2);
                ImGui::Text("  Toggle button ");

                ImGui::Dummy(ImVec2(0, 6));

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::Combo("##comboAimingType", &options::aimingType, aimingTypes, IM_ARRAYSIZE(aimingTypes));
                ImGui::SameLine();
                ImGui::PopStyleColor(2);
                ImGui::PopStyleVar(2);
                ImGui::Text("  Aiming type");

                ImGui::Dummy(paddings);
                ToggleButton("asdsad", &menu::bSmooth);
                ImGui::SameLine();
                ImGui::Text("  Smooth");

                ImGui::Dummy(paddings);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::SliderFloat("##sliderSmooth", &options::smoothness, 1.0f, 10.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::PopStyleColor();
                ImGui::Text("  Smoothness");

                ImGui::Dummy(paddings);
                ToggleButton("zxcmvs", &menu::bFOV);
                ImGui::SameLine();
                ImGui::Text("  Draw FOV");

                ImGui::Dummy(paddings);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::SliderFloat("##sliderRadiusFOV", &options::radiusFOV, 0.0f, 300.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::PopStyleColor();
                ImGui::Text("  FOV radius");

                ImGui::EndChild();
                ///////////////////////////////////
                ImGui::Dummy(ImVec2(0, 20));
                ///////////////////////////////////
                ImGui::BeginChild("TriggerbotFrame", ImVec2(404, 120), true);

                ImGui::Indent(10.f);

                ImGui::Dummy(ImVec2(0, 15));
                ToggleButton("asasda", &menu::bEnableTriggerbot);
                ImGui::SameLine();
                ImGui::Text("  Enable Triggerbot");

                ImGui::Dummy(paddings);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::SliderInt("##triggerbotDelay ", &options::triggerbotDelay, 1, 100, "%d ms", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::PopStyleColor();
                ImGui::Text("  Delay");

                ImGui::EndChild();

                ImGui::PopStyleVar();
            }
            else if (selectedTab == 1) {
                ImVec2 mainWindowPos = ImGui::GetWindowPos();
                ImVec2 mainWindowSize = ImGui::GetWindowSize();

                ImVec2 espWindowPos = ImVec2(mainWindowPos.x + mainWindowSize.x + 40, mainWindowPos.y);

                ImGui::Begin("Esp preview", nullptr, ImGuiWindowFlags_NoResize);
                ImGui::SetWindowPos(espWindowPos, ImGuiCond_FirstUseEver);
                ImGui::SetWindowSize(ImVec2(230, 400), ImGuiCond_FirstUseEver);

                ImVec2 espFramePos = ImGui::GetWindowPos();
                ImVec2 espFrameSize = ImGui::GetWindowSize();

                ImDrawList* drawList = ImGui::GetWindowDrawList();

                if (menu::bEnableBox2D) {
                    if (menu::bEnableBox2DLines) {
                        drawList->AddRect(ImVec2(espFramePos.x + 20, espFramePos.y + 30), ImVec2(espFramePos.x + espFrameSize.x - 20, espFramePos.y + espFrameSize.y - 20), (ImColor)options::boxes2DLinesColor, options::box2DRounding, 0, 1.4f);
                    }
                    if (menu::bEnableBox2DFill) {
                        drawList->AddRectFilled(ImVec2(espFramePos.x + 20, espFramePos.y + 30), ImVec2(espFramePos.x + espFrameSize.x - 20, espFramePos.y + espFrameSize.y - 20), (ImColor)options::boxes2DFillColor, options::box2DRounding);
                    }
                } 

                if (menu::bEnableLines) {
                    drawList->AddLine(ImVec2(espFramePos.x + espFrameSize.x / 2, espFramePos.y + espFrameSize.y), ImVec2(espFramePos.x + espFrameSize.x / 2, espFramePos.y + espFrameSize.y - 20), (ImColor)options::snapLinesColor, 2.f);
                }

                if (menu::bEnableHealth) {
                    drawList->AddRectFilled(ImVec2(espFramePos.x + 20, espFramePos.y + 30), ImVec2(espFramePos.x + 13, espFramePos.y + espFrameSize.y - 20), ImColor(0.0f, 1.0f, 0.0f, 1.f));
                }

                ImGui::End();

                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
                ImGui::BeginChild("TriggerbotFrame", ImVec2(404, 340), true);

                ImGui::Indent(14.f);

                ImGui::Dummy(paddings);
                ToggleButton("##enableESP", &menu::bEnableESP);
                ImGui::SameLine();
                ImGui::Text("  Enable ESP ");

                ImGui::Dummy(paddings);

                ToggleButton("##enable2dbox", &menu::bEnableBox2D);
                ImGui::SameLine();
                ImGui::Text("  2D Box ");

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushFont(iconsFont);
                if (ImGui::Button(ICON_FA_COG, ImVec2(20, 22))) box2dsettings = !box2dsettings;
                ImGui::PopFont();
                ImGui::PopStyleColor(2);

                if (box2dsettings) {
                    ImGui::Begin("2D box settings", nullptr, ImGuiWindowFlags_NoResize);
                    ImGui::SetWindowSize(ImVec2(200, 300), ImGuiCond_Always);

                    ImGui::Indent(10.f);
                    ImGui::Dummy(paddings);

                    ToggleButton("asdsad", &menu::bEnableBox2DFill);
                    ImGui::SameLine();
                    ImGui::Text("  Fill");

                    ImGui::Dummy(paddings);

                    ToggleButton("asdsdsdad", &menu::bEnableBox2DLines);
                    ImGui::SameLine();
                    ImGui::Text("  Lines");

                    ImGui::Dummy(paddings);

                    ImGui::Text("Rounding");
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                    ImGui::SliderFloat("##Rouning", &options::box2DRounding, 0.f, 50.f, "%.0f", 0);
                    ImGui::PopStyleColor();

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes2DFillColor, 0);
                    ImGui::SameLine();
                    ImGui::Text(" Fill color");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes2DLinesColor, 1);
                    ImGui::SameLine();
                    ImGui::Text(" Lines color");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes2DLinesColorTeam, 2);
                    ImGui::SameLine();
                    ImGui::Text(" Lines color team");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes2DFillColorTeam, 3);
                    ImGui::SameLine();
                    ImGui::Text(" Fill color team");

                    ImGui::End();
                }

                ImGui::Dummy(ImVec2(paddings.x, paddings.y - 2));

                ToggleButton("##enable3dbox", &menu::bEnableBox3D);
                ImGui::SameLine();
                ImGui::Text("  3D Box ");

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushFont(iconsFont);
                if (ImGui::Button(ICON_FA_COG" ", ImVec2(20, 22))) box3dsettings = !box3dsettings;
                ImGui::PopFont();
                ImGui::PopStyleColor(2);

                if (box3dsettings) {
                    ImGui::Begin("3D box settings", nullptr, ImGuiWindowFlags_NoResize);
                    ImGui::SetWindowSize(ImVec2(200, 140), ImGuiCond_Always);

                    ImGui::Indent(10.f);

                    ImGui::Dummy(paddings);

                    ToggleButton("##animation", &menu::bEnableBox3DAnim);
                    ImGui::SameLine();
                    ImGui::Text("  Animation");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes3DLinesColor, 3);
                    ImGui::SameLine();
                    ImGui::Text(" Lines color");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::boxes3DLinesColorTeam, 7);
                    ImGui::SameLine();
                    ImGui::Text(" Lines color team");
                    ImGui::End();
                }

                ImGui::Dummy(ImVec2(paddings.x, paddings.y - 2));

                ToggleButton("##enablesnaplines", &menu::bEnableLines);
                ImGui::SameLine();
                ImGui::Text("  Snaplines ");
                ImGui::SameLine();
                
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushFont(iconsFont);
                if (ImGui::Button(ICON_FA_COG"    ", ImVec2(20, 22))) snaplinessettings = !snaplinessettings;
                ImGui::PopFont();
                ImGui::PopStyleColor(2);

                if (snaplinessettings) {
                    ImGui::Begin("Snaplines settings", nullptr, ImGuiWindowFlags_NoResize);
                    ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_Always);

                    ImGui::Indent(10.f);

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::snapLinesColor, 12);
                    ImGui::SameLine();
                    ImGui::Text(" Color");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::snapLinesColorTeam, 10);
                    ImGui::SameLine();
                    ImGui::Text(" Color team");

                    ImGui::End();
                }

                ImGui::Dummy(paddings);

                ToggleButton("##enablehealth", &menu::bEnableHealth);
                ImGui::SameLine();
                ImGui::Text("  Health ");

                ImGui::Dummy(paddings);

                ToggleButton("##enabledirections", &menu::bEnableDirection);
                ImGui::SameLine();
                ImGui::Text("  Directions ");
                ImGui::SameLine();
                ShowColorPickerButton(&options::directionColor, 6);

                ImGui::Dummy(paddings);

                ToggleButton("##enableskeleton", &menu::bEnableSkeleton);
                ImGui::SameLine();
                ImGui::Text("  Skeleton  ");
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushFont(iconsFont);
                if (ImGui::Button(ICON_FA_COG"   ", ImVec2(20, 22))) skeletonsettings = !skeletonsettings;
                ImGui::PopFont();
                ImGui::PopStyleColor(2);

                if (skeletonsettings) {
                    ImGui::Begin("Skeleton settings", nullptr, ImGuiWindowFlags_NoResize);
                    ImGui::SetWindowSize(ImVec2(200, 130), ImGuiCond_Always);
                    ImGui::Indent(10.f);

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::skeletonColor, 8);
                    ImGui::SameLine();
                    ImGui::Text("Color");

                    ImGui::Dummy(paddings);

                    ShowColorPickerButton(&options::skeletonColorTeam, 9);
                    ImGui::SameLine();
                    ImGui::Text("Color team");

                    ImGui::End();
                }

                ImGui::Dummy(ImVec2(paddings.x, paddings.y - 2));

                ToggleButton("##enablename", &menu::bEnableName);
                ImGui::SameLine();
                ImGui::Text("  Name ");
                ImGui::SameLine();

                ImGui::Dummy(paddings);
                ImGui::Dummy(paddings);

                ToggleButton("##teamvisible", &menu::bTeamVisible);
                ImGui::SameLine();
                ImGui::Text("  Team visible ");
                ImGui::SameLine();

                ImGui::EndChild();
                ImGui::PopStyleVar();
            }
            else if (selectedTab == 2) {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

            ImGui::BeginChild("##savecfgframe", ImVec2(404, 100), true);
            ImGui::Indent(14.f);
                ImGui::Dummy(ImVec2(0, 20));

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                ImGui::InputText("##configname", configName, IM_ARRAYSIZE(configName));

                ImGui::SameLine();
                ImGui::PopStyleColor();
                ImGui::Text("  Config name");

                ImGui::Dummy(paddings);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));
                if (ImGui::Button("Save config", ImVec2(130, 30))) {
                    config.saveConfig(std::string (configName));
                    config.update();
                    selectedConfig = config.configFiles[0];
                }

                ImGui::EndChild();

                ImGui::Dummy(ImVec2(0, 25));

                ImGui::BeginChild("##loadcfgframe", ImVec2(404, 190), true);
                ImGui::Indent(14.f);
                ImGui::Dummy(ImVec2(0, 20));

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
                if (ImGui::Combo("##selectconfig", &selectedConfigIndex, config.configNames.data(), config.configNames.size())) {
                    if (selectedConfigIndex >= 0 && selectedConfigIndex < config.configFiles.size()) {
                        selectedConfig = config.configFiles[selectedConfigIndex];
                    }
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
                ImGui::Text(" Choose config");
                ImGui::PopStyleColor();

                ImGui::Dummy(paddings);

                if (ImGui::Button("Load config", ImVec2(130, 30))) {
                    config.loadConfig(selectedConfig);
                }

                ImGui::Dummy(paddings);

                if (ImGui::Button("Delete config", ImVec2(130, 30))) {
                    if (!config.configFiles.empty() && !selectedConfig.empty()) {
                        config.deleteConfig(selectedConfig);
                        config.update();
                        selectedConfig = config.configFiles[0];
                    }                    
                }

                ImGui::Dummy(paddings);

                if (ImGui::Button("Update configs", ImVec2(130, 30))) {
                      config.update();
                      selectedConfig = config.configFiles[0];
                }

                ImGui::EndChild();

                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
            else if (selectedTab == 3) {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
                ImGui::BeginChild("##settingsframe", ImVec2(404, 300), true);
                ImGui::Dummy(paddings);
                ImGui::Indent(14.f);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));

                if (ImGui::Button("Toggle menu key", ImVec2(140, 27))) {
                    waitingForToggleKey = true; 
                }

                ImGui::Dummy(paddings);

                if (waitingForToggleKey) {
                    ImGui::Text("Press key...");

                    for (int key = 0; key < 256; ++key) { 
                        if (GetAsyncKeyState(key) & 0x8000) {
                            menu::bToggleButton = key; 
                            waitingForToggleKey = false; 
                            break;
                        }
                    }
                }

                ImGui::Text("Key: % s", ImGui::GetKeyName(static_cast<ImGuiKey>(menu::bToggleButton)));

                ImGui::Dummy(paddings);

                if (ImGui::Button("End menu key", ImVec2(140, 27))) {
                    waitingForEndKey = true; 
                }

                ImGui::Dummy(paddings);

                if (waitingForEndKey) {
                    ImGui::Text("Press key...");

                    for (int key = 0; key < 256; ++key) { 
                        if (GetAsyncKeyState(key) & 0x8000) {
                            menu::bEndButton = key; 
                            waitingForEndKey = false; 
                            break;
                        }
                    }
                }

                ImGui::Text("Key: % s", ImGui::GetKeyName(static_cast<ImGuiKey>(menu::bEndButton)));

                ImGui::PopStyleColor();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }

            ImGui::PopFont();
            ImGui::EndChild();

            ImGui::End();
}