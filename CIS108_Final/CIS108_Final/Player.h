#pragma once
#include <string>
#include "commonHeader.h"
#include <Windows.h>
class Player
{
public:
	float fPlayerX;
	float fPlayerY;
	float fPlayerA;

	float iPlayerHealth;

	float PlayerSpeed;

	bool OnPad;

	Player();
	Player(std::string savedGame);
	void Move(unsigned short key, float time, Settings currentSettings, HWND consoleWindow);
	void TakeDamage(float damage, HWND consoleWindow);
	void ResetPad();
	std::string GetData();
};