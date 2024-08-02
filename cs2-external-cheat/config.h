#pragma once
#include <string>

class Config {
public:
	std::string configPath;
	std::string offsetsPath;
	std::vector<std::string> configFiles;
	std::vector<const char*> configNames;
public:
	bool downloadOffsetsFromGithub();
	bool createConfigFolder();
	bool createOffsetsFolder();
	void vectorStrToChar();
	void getDocumentsFolderPath();
	void getConfigFiles();
	bool deleteConfig(std::string configName);
	bool loadConfig(std::string configName);
	bool saveConfig(std::string configName);
	bool initialize();
	void update();
};

inline Config config;