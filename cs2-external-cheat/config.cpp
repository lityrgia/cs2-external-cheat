#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <filesystem>
#include "json.h"
#include "config.h"
#include "options.h"

using json = nlohmann::json;

void Config::getDocumentsFolderPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
        this->configPath = std::string(path);
    }
    else {
        std::cout << "[cheat]: Can't get documents path" << '\n';
    }
}

bool Config::createConfigFolder() {
    HRESULT result = SHCreateDirectoryExA(NULL, (this->configPath + "\\Vireless").c_str(), NULL);
    this->configPath = configPath + "\\Vireless";

    if (result == ERROR_SUCCESS) {
        std::cout << "[cheat]: Config folder created " << '\n';
        return true;
    }
    else if (result == ERROR_ALREADY_EXISTS) {
        std::cout << "[cheat]: Config folder already created" << '\n';
        return true;
    }
    else {
        std::cout << "[cheat]: Can't create config folder " << '\n';
        return false;
    }
}

bool Config::downloadOffsetsFromGithub() {
    // Создаем команду для вызова PowerShell скрипта
    std::string urlOffsets = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json";
    std::string commandOffsets = "Invoke-WebRequest -Uri \"" + urlOffsets + "\" -OutFile \"" + this->offsetsPath + "\\offsetsDump.json" + "\"";

    std::string urlClient = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client_dll.json";
    std::string commandClient = "Invoke-WebRequest -Uri \"" + urlClient + "\" -OutFile \"" + this->offsetsPath + "\\clientDump.json" + "\"";

    std::string fullCommand = "powershell -ExecutionPolicy Bypass -Command \"" + commandOffsets + "\"";

    int result = std::system(fullCommand.c_str());

    fullCommand = "powershell -ExecutionPolicy Bypass -Command \"" + commandClient + "\"";

    result = std::system(fullCommand.c_str());

    if (result != 0) {
        std::cerr << "[cheat]: Failed to download offsets " << '\n';
        return false;
    }

    return true;
}

bool Config::createOffsetsFolder(){
    HRESULT result = SHCreateDirectoryExA(NULL, (this->configPath + "\\Offsets").c_str(), NULL);

    if (result == ERROR_SUCCESS) {
        std::cout << "[cheat]: Offsets folder created " << '\n';
    }
    else if (result == ERROR_ALREADY_EXISTS) {
        std::cout << "[cheat]: Offsets folder already created" << '\n';
    }
    else {
        std::cout << "[cheat]: Can't create offsets folder " << '\n';
        return false;
    }

    this->offsetsPath = this->configPath + "\\Offsets";

    std::ofstream clientDump(this->offsetsPath + "\\clientDump.json");
    std::ofstream offsetsDump(this->offsetsPath + "\\offsetsDump.json");

    if (clientDump.is_open() && offsetsDump.is_open()) {
        clientDump.close();
        offsetsDump.close();
    }

    return true;
}

void Config::getConfigFiles() {
    try {
        if (!std::filesystem::exists(this->configPath) || !std::filesystem::is_directory(this->configPath)) {
            std::cerr << "[cheat]: Path doesn't exist " << configPath << std::endl;
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(this->configPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                this->configFiles.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[cheat]: Error with filesystem " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "[cheat]: Unable to loop through config files " << e.what() << '\n';
    }
}

void Config::vectorStrToChar() {
    for (const auto& file : config.configFiles) {
        this->configNames.push_back(file.c_str());
    }
}

void Config::update(){
    this->configFiles.clear();
    this->configNames.clear();
    this->getConfigFiles();
    this->vectorStrToChar();
}

json imvec4ToJson(const ImVec4& color) {
    return json{ {"r", color.x}, {"g", color.y}, {"b", color.z}, {"a", color.w} };
}

bool Config::saveConfig(std::string configName) {
    json config;

    config["name"] = "Vireless";
    config["version"] = "1.0";
    config["config-name"] = "test";
    config["menu"] = {
        {"Aim", menu::bEnableAim},
        {"FOV", menu::bFOV},
        {"Smooth", menu::bSmooth},
        {"Triggerbot", menu::bEnableTriggerbot},
        {"Esp", menu::bEnableESP},
        {"2D-box", menu::bEnableBox2D},
        {"2D-box-lines", menu::bEnableBox2DLines},
        {"2D-box-fill", menu::bEnableBox2DFill},
        {"3D-box", menu::bEnableBox3D},
        {"3D-box-anim", menu::bEnableBox3DAnim},
        {"Name", menu::bEnableName},    
        {"Health", menu::bEnableHealth},
        {"Snaplines", menu::bEnableLines},
        {"Skeleton", menu::bEnableSkeleton},
        {"Direction", menu::bEnableDirection},
        {"Team-visible", menu::bTeamVisible},
        {"Toggle-button", menu::bToggleButton},
        {"End-button", menu::bEndButton},
    };
    config["options"] = {
        {"Radius-fov", options::radiusFOV},
        {"Aiming-type", options::aimingType},
        {"Triggerbot-delay", options::triggerbotDelay},
        {"Smoothness", options::smoothness},
        {"2d-box-rounding", options::box2DRounding},

        {"2d-box-lines-color", imvec4ToJson(options::boxes2DLinesColor)},
        {"2d-box-fill-color", imvec4ToJson(options::boxes2DFillColor)},
        {"2d-box-lines-team-color", imvec4ToJson(options::boxes2DLinesColorTeam)},
        {"2d-box-fill-team-color", imvec4ToJson(options::boxes2DFillColorTeam)},

        {"3d-box-lines-color", imvec4ToJson(options::boxes3DLinesColor)},
        {"3d-box-lines-team-color", imvec4ToJson(options::boxes3DLinesColorTeam)},

        {"Snaplines-color-team", imvec4ToJson(options::snapLinesColorTeam)},
        {"Snaplines-color", imvec4ToJson(options::snapLinesColor)},
        {"Direction-color", imvec4ToJson(options::directionColor)},
        {"Skeleton-color", imvec4ToJson(options::skeletonColor)},
        {"Skeleton-team-color", imvec4ToJson(options::skeletonColorTeam)},
    };

    if (configName.empty()) {
        configName = "config";
    }

    std::ofstream configFile(configPath + "\\" + configName + ".json");

    if (configFile.is_open()) {
        configFile << config.dump(4);
        configFile.close();

        std::cout << "[cheat]: Config " + std::string(configName) + ".json saved" << '\n';
    }
    else {
        std::cout << "[cheat]: Can't save " + std::string(configName) << ".json" << '\n';
    }

    return true;
}

ImVec4 jsonToImvec4(const json& j) {
    return ImVec4(j.at("r").get<float>(), j.at("g").get<float>(), j.at("b").get<float>(), j.at("a").get<float>());
}

bool Config::loadConfig(std::string configName) {
    std::ifstream configFile(configPath + "\\" + configName);

    if (configFile.is_open()) {
        json config;
        configFile >> config;
        configFile.close();

        menu::bEnableAim = config["menu"]["Aim"];
        menu::bEnableTriggerbot = config["menu"]["Triggerbot"];
        menu::bSmooth = config["menu"]["Smooth"];
        menu::bFOV = config["menu"]["FOV"];

        menu::bEnableESP = config["menu"]["Esp"];
        menu::bEnableBox2D = config["menu"]["2D-box"];
        menu::bEnableBox2DFill = config["menu"]["2D-box-fill"];
        menu::bEnableBox2DLines = config["menu"]["2D-box-lines"];
        menu::bEnableBox3D = config["menu"]["3D-box"];
        menu::bEnableBox3DAnim = config["menu"]["3D-box-anim"];
        menu::bEnableLines = config["menu"]["Snaplines"];
        menu::bEnableSkeleton = config["menu"]["Skeleton"];
        menu::bEnableName = config["menu"]["Name"];
        menu::bEnableDirection = config["menu"]["Direction"];
        menu::bEnableHealth = config["menu"]["Health"];
        menu::bTeamVisible = config["menu"]["Team-visible"];

        menu::bToggleButton = config["menu"]["Toggle-button"];
        menu::bEndButton = config["menu"]["End-button"];

        options::radiusFOV = config["options"]["Radius-fov"];
        options::aimingType = config["options"]["Aiming-type"];
        options::smoothness = config["options"]["Smoothness"];
        options::triggerbotDelay = config["options"]["Triggerbot-delay"];
        options::box2DRounding = config["options"]["2d-box-rounding"];

        options::boxes2DLinesColor = jsonToImvec4(config["options"]["2d-box-lines-color"]);
        options::boxes2DFillColor = jsonToImvec4(config["options"]["2d-box-fill-color"]);
        options::boxes2DLinesColorTeam = jsonToImvec4(config["options"]["2d-box-lines-team-color"]);
        options::boxes2DFillColorTeam = jsonToImvec4(config["options"]["2d-box-fill-team-color"]);

        options::boxes3DLinesColorTeam = jsonToImvec4(config["options"]["3d-box-lines-team-color"]);
        options::boxes3DLinesColor = jsonToImvec4(config["options"]["3d-box-lines-color"]);

        options::snapLinesColorTeam = jsonToImvec4(config["options"]["Snaplines-color-team"]);
        options::snapLinesColor = jsonToImvec4(config["options"]["Snaplines-color"]);
        options::directionColor = jsonToImvec4(config["options"]["Direction-color"]);

        options::skeletonColor = jsonToImvec4(config["options"]["Skeleton-color"]);
        options::skeletonColorTeam = jsonToImvec4(config["options"]["Skeleton-team-color"]);

        std::cout << "[cheat]: Config " + configName + " loaded" << '\n';
    }
    else {
        std::cout << "[cheat]: Can't load config " + configName << '\n';
    }
}

bool Config::deleteConfig(std::string configName) {
    if (std::filesystem::remove(configPath + "\\" + configName)) {
        std::cout << "[cheat]: Config " << configName << " deleted" << '\n';
    }
    else {
        std::cout << "[cheat]: Can't delete " << configName << '\n';
    }
}

bool Config::initialize() {
    this->getDocumentsFolderPath();
    if (!this->createConfigFolder()) return false;
    if (!this->createOffsetsFolder()) return false;
    this->getConfigFiles();
    this->vectorStrToChar();

    return true;
}