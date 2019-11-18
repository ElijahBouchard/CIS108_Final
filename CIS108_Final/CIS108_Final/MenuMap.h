#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include "Settings.h"
#include "Player.h"
#include "commonHeader.h"
class MenuMap
{
public:
	int state;
	std::string currentSelection;

	MenuMap();
	void PrintMap(std::string* menu);
	void MoveSelection(unsigned short key);
	std::string Select();
	void ShowControls();
	void ShowSettings(Settings settings, Player player, int selection);
	void Back();
	void ShowSaves(int num);
};

