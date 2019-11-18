#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "commonHeader.h"
#include "Player.h"
#include "MenuMap.h"

class Settings
{
public:
	int cScreenWidth;
	int cScreenHeight;

	int cMapWidth;
	int cMapHeight;

	float fFOV;
	float fDepth;

	float Sensitivity;

	int mapNumber;

	std::wstring map;

	Settings();
	Settings(std::string savedGame);
	std::string LoadGame(MenuMap menu);
	void LoadMap();
	int NextLevel(Player currentPlayer, HWND consoleWindow);
	std::string GetData();
	void SaveGame(Player currentPlayer, HWND handle);
};

