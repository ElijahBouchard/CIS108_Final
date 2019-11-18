#define _WIN32_WINNT 0x0500
#include <iostream>
#include <Windows.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "conio.h"
#include "MenuMap.h"
#include "Settings.h"
#include "Player.h"
using namespace std;

int main()
{
	//Creates Settings Instance
	Settings settings = Settings::Settings();

	//Creates Player Instance
	Player player = Player::Player();

	//Start Menu
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	string ans = "";
	bool start = false;

	//Creates menu instance
	MenuMap menu = MenuMap::MenuMap();
	string selection;

	while (!start)
	{
		char key = _getch();
		if (toupper(key) == 'S')
		{
			menu.MoveSelection('S');
		}
		if (toupper(key) == 'W')
		{
			menu.MoveSelection('W');
		}
		if (GetAsyncKeyState((unsigned short)VK_RETURN) & 0x8000)
		{
			selection = menu.Select();
			if (selection == "start game")
			{
				int count = 0;
				menu.ShowSaves(count);
				while (1)
				{
					char key = _getch();
					if (toupper(key) == 'S')
					{
						if (count == 0)
						{
							count = 2;
						}
						else
						{
							count--;
						}
						menu.ShowSaves(count);
					}
					else if (toupper(key) == 'W')
					{
						if (count == 2)
						{
							count = 0;
						}
						else
						{
							count++;
						}
						menu.ShowSaves(count);
					}

					if (GetAsyncKeyState((unsigned short)VK_RETURN) & 0x8000)
					{
						if (menu.Select() == "back")
						{
							break;
						}
						else if (menu.Select() == "load save")
						{
							string ans;
							ans = settings.LoadGame(menu);
							
							if (ans != "")
							{
								settings = Settings::Settings(ans);
								player = Player::Player(ans);
								start = true;
								break;
							}
							else
							{
								if (GetAsyncKeyState((unsigned short)VK_RETURN) & 0x8000)
								{
									break;
								}
							}
						}
						else if (menu.Select() == "new game")
						{
							start = true;
							break;
						}
					}
				}
				menu.Back();
			}
			else if (selection == "how to play")
			{
				menu.ShowSettings(settings, player, 0);
			}
			else if (selection == "controls")
			{
				menu.ShowControls();
			}
			else if (selection == "back")
			{
				menu.Back();
			}
		}
	}

	//Create Screen Buffer
	wchar_t *screen = new wchar_t[settings.cScreenWidth*settings.cScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//Time Points
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//Game Loop
	while (1)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		//Controls
		//Handle CCW Rotation

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			player.Move((unsigned short)'A', fElapsedTime * settings.Sensitivity, settings, consoleWindow);
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			player.Move((unsigned short)'D', fElapsedTime * settings.Sensitivity, settings, consoleWindow);
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			player.Move((unsigned short)'W', fElapsedTime * player.PlayerSpeed, settings, consoleWindow);
			settings.NextLevel(player, consoleWindow);
			if (GetAsyncKeyState((unsigned short)VK_SHIFT) & 0x8000)
			{
				player.Move((unsigned short)'W', fElapsedTime * player.PlayerSpeed * 1.05, settings, consoleWindow);
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			player.Move((unsigned short)'S', fElapsedTime * player.PlayerSpeed, settings, consoleWindow);
			settings.NextLevel(player, consoleWindow);
		}
		if (GetAsyncKeyState((unsigned short)'O') & 0x8000)
		{
			settings.SaveGame(player, consoleWindow);
		}
		if (GetAsyncKeyState((unsigned short)'P') & 0x8000)
		{
			return 0;
		}

		//Wall and floor shading
		for (int x = 0; x < settings.cScreenWidth; x++)
		{
			//For each column, calculate the projected ray angle into world space
			float fRayAngle = (player.fPlayerA - settings.fFOV / 2.0f) + ((float)x / (float)settings.cScreenWidth) * settings.fFOV;

			float fDistanceToWall = 0;
			bool HitWall = false;
			bool Boundary = false;

			bool HitTrap = false;
			bool HitDoor = false;
			bool HitPad = false;

			float fEyeX = sinf(fRayAngle); //Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			while (!HitWall && fDistanceToWall < settings.fDepth)
			{
				fDistanceToWall += 0.1f;

				int TestX = (int)(player.fPlayerX + fEyeX * fDistanceToWall);
				int TestY = (int)(player.fPlayerY + fEyeY * fDistanceToWall);

				//Test if ray is out of bounds
				if (TestX < 0 || TestX >= settings.cMapWidth || TestY < 0 || TestY >= settings.cMapHeight)
				{
					HitWall = true;
					fDistanceToWall = settings.fDepth;
				}
				else
				{
					//Ray is inbounds so test to see if the ray cell is a wall block
					if (settings.map[TestY * settings.cMapWidth + TestX] == '#')
					{
						HitWall = true;

						vector<pair<float, float>> p; //distance, dot product

						for (int tx = 0; tx < 2; tx++)
						{
							for (int ty = 0; ty < 2; ty++)
							{
								float vy = (float)TestY + ty - player.fPlayerY;
								float vx = (float)TestX + tx - player.fPlayerX;

								float d = sqrt(vx*vx + vy * vy);

								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}
						}

						//Sort pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right)
						{
							return left.first < right.first;
						});

						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound)
						{
							Boundary = true;
						}
						if (acos(p.at(1).second) < fBound)
						{
							Boundary = true;
						}
						/*if (acos(p.at(2).second) < fBound)
						{
							Boundary = true;
						}*/
					}
					else if (settings.map[TestY * settings.cMapWidth + TestX] == 'X')
					{
						HitTrap = true;
					}
					else if (settings.map[TestY * settings.cMapWidth + TestX] == '|')
					{
						HitDoor = true;
					}
					else if (settings.map[TestY * settings.cMapWidth + TestX] == 'K')
					{
						HitPad = true;
					}
				}
			}

			//Calculate distance to ceiling and floor
			int cCeiling = (float)(settings.cScreenHeight / 2.0) - settings.cScreenHeight / ((float)fDistanceToWall);
			int cFloor = settings.cScreenHeight - cCeiling;

			//Determine Shade of wall
			short sShade = ' ';

			if (HitDoor)
			{
				sShade = '#';
			}
			else if (fDistanceToWall <= settings.fDepth / 4.0f)
			{
				sShade = 0x2588; //Very Close
			}
			else if (fDistanceToWall < settings.fDepth / 3.0f)
			{
				sShade = 0x2593;
			}
			else if (fDistanceToWall < settings.fDepth / 2.0f)
			{
				sShade = 0x2592;
			}
			else if (fDistanceToWall < settings.fDepth)
			{
				sShade = 0x2591;
			}
			else
			{
				sShade = 0x2591; //Too far away
			}

			if (Boundary)
			{
				sShade = '|'; //Black it out
			}

			for (int y = 0; y < settings.cScreenHeight; y++)
			{
				if (y < cCeiling)
				{
					screen[y*settings.cScreenWidth + x] = ' ';
				}
				else if (y > cCeiling && y <= cFloor)
				{
					screen[y*settings.cScreenWidth + x] = sShade;
				}
				else
				{
					//Shade floor based on distance
					float b = 1.0f - (((float)y - settings.cScreenHeight / 2.0f) / ((float)settings.cScreenHeight / 2.0f));
					short oShade = ' ';

					/*if (b < 0.25)
					{
						oShade = '#'; //Almost full
					}
					else if (b < 0.5)
					{
						oShade = 'X';
					}
					else if (b < 0.75)
					{
						oShade = '.';
					}
					else if (b < 0.9)
					{
						oShade = '-';
					}*/
					if (HitTrap)
					{
						if (b < 0.50)
						{
							oShade = '.';
						}
						else if (b < 0.9)
						{
							oShade = '-';
						}
						else
						{
							oShade = ' ';
						}
					}
					else if (HitPad)
					{
						if (b < 0.25)
						{
							oShade = '.';
						}
						else if (b < 0.9)
						{
							oShade = 'U';
						}
						else
						{
							oShade = ' ';
						}
					}
					else if (b < 0.9)
					{
						oShade = '.';
					}
					else
					{
						oShade = ' '; //Too far away
					}

					screen[y*settings.cScreenWidth + x] = oShade;
				}
			}
		}

		
		//Display stats
		swprintf_s(screen, 45, L" HEALTH=%3.2f, FPS=%3.2f", player.iPlayerHealth, 1.0f / fElapsedTime);

		//Display Map
		/*for (int nx = 0; nx < settings.cMapWidth; nx++)
		{
			for (int ny = 0; ny < settings.cMapWidth; ny++)
			{
				screen[(ny + 1) * settings.cScreenWidth + nx] = settings.map[ny *settings.cMapWidth + (settings.cMapWidth - nx - 1)];
			}
		}

		screen[((int)player.fPlayerY + 1) * settings.cScreenWidth + (int)(settings.cMapWidth - player.fPlayerX)] = 'P';*/

		screen[settings.cScreenWidth * settings.cScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, settings.cScreenWidth * settings.cScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}