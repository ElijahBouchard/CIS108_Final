#include "Settings.h"

Settings::Settings()
{
	cScreenWidth = 120;
	cScreenHeight = 40;

	cMapWidth = 16;
	cMapHeight = 16;

	fFOV = 3.14159 / 4.0;
	fDepth = 16.0f;

	Sensitivity = 1.25f;

	map = L"";
	mapNumber = 1;

	LoadMap();
}

Settings::Settings(std::string savedGame)
{
	cScreenWidth = 120;
	cScreenHeight = 40;

	cMapWidth;
	cMapHeight;

	fFOV = 3.14159 / 4.0;
	fDepth = 16.0f;

	Sensitivity = 1.25f;

	map = L"";
	mapNumber;

	std::string filename = "SavedGame" + savedGame + ".txt";
	std::ifstream infile(filename);

	std::string line;

	for (int i = 0; i < 3; i++)
	{
		std::getline(infile, line);
		if (i == 0)
		{
			cMapWidth = stoi(line);
		}
		else if (i == 1)
		{
			cMapHeight = stoi(line);
		}
		else if (i == 2)
		{
			mapNumber = stoi(line);
		}
	}
	LoadMap();
}
std::string Settings::LoadGame(MenuMap menu)
{
	std::string savesMap[10];
	std::string ans = "";
	int count = 0;

	system("CLS");
	bool done = false;
	while (!done)
	{
		for (int i = 1; i < 100; i++)
		{
			std::string filename = "SavedGame" + std::to_string(i) + ".txt";
			std::ifstream outfile(filename);

			if (!outfile.fail())
			{
				savesMap[i - 1] = filename;
				count++;
			}
			else
			{
				if (count == 0)
				{
					std::cout << "No saves found, press w or s.";
				}
				done = true;
				break;
			}
		}
	}

	menu.PrintMap(savesMap);

	if (count > 0)
	{
		std::cout << "Enter number of save (ex: 1): ";
		std::cin >> ans;
	}

	return ans;
}
void Settings::LoadMap()
{
	//Create Map
	if (mapNumber == 1)
	{
		map += L"#|##############";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#....#######...#";
		map += L"#..............#";
		map += L"#...........####";
		map += L"#...........#XX#";
		map += L"#..#..#######XX#";
		map += L"#..#XX#######XX#";
		map += L"#..#...........#";
		map += L"#XX#...........#";
		map += L"#..#############";
		map += L"#..XX........K.#";
		map += L"#..XX..........#";
		map += L"################";
	}
	else if (mapNumber == 2)
	{
		cMapWidth = 31;
		cMapHeight = 16;
		map = L"";
		map += L"############################|##";
		map += L"#.............................#";
		map += L"####..#######################.#";
		map += L"#XX#..#XXXXXX.................#";
		map += L"#..#..#########################";
		map += L"#XX#...........#..............#";
		map += L"#..#..##########....####......#";
		map += L"#...........#XX#.....X........#";
		map += L"#..#..#######XX#..............#";
		map += L"#..#XX#######XX#...############";
		map += L"#..#...............#.....XX..K#";
		map += L"#XX#...............#.##########";
		map += L"#..############...............#";
		map += L"#..XX.........#......#...#....#";
		map += L"#..XX.........#......#........#";
		map += L"###############################";
	}
	else if (mapNumber == 3)
	{
		cMapWidth = 58;
		cMapHeight = 16;
		map = L"";
		map += L"##########################################################";
		map += L"#........................................................#";
		map += L"####..#######################################.#..........#";
		map += L"#XX#..#XXXXXX.............................#...#..........#";
		map += L"#..#..#######################.#############.###.##########";
		map += L"#XX#...........#............................###.#........#";
		map += L"#..#..##########....###########.............#...#........#";
		map += L"#...........#XX#.....X........#.............#.###.########";
		map += L"#..#..#######XX#..............#....####..................#";
		map += L"#..#XX#######XX#...############..........................#";
		map += L"#..#...............#.....XX............K....#######......#";
		map += L"#XX#...............#.########....####....................#";
		map += L"#..############..................#..#.....#....#####.....#";
		map += L"#..XX.........#......#...#.......####.....#........#.....#";
		map += L"#..XX.........#......#....................#....###.#.....#";
		map += L"##################################################|#######";
	}
}
int Settings::NextLevel(Player currentPlayer, HWND consoleWindow)
{
	if (currentPlayer.OnPad && map[(int)currentPlayer.fPlayerY * cMapWidth + (int)currentPlayer.fPlayerX] == '|')
	{
		currentPlayer.fPlayerX = 3.0f;
		currentPlayer.fPlayerY = 3.0f;
		currentPlayer.ResetPad();
		mapNumber++;
		if (mapNumber == 4)
		{
			int saveGameBox = MessageBox(
				consoleWindow,
				(LPCWSTR)L"You Finished The Game!",
				(LPCWSTR)L"Exiting Game.",
				MB_ICONEXCLAMATION | MB_OK
			);
			exit(EXIT_SUCCESS);
		}
		LoadMap();
	}

	return 1;
}
std::string Settings::GetData()
{
	std::string data =
		std::to_string(cMapWidth) + "\n" +
		std::to_string(cMapHeight) + "\n" +
		std::to_string(mapNumber) + "\n";

	return data;
}
std::string generate()
{
	std::string generated = "";

	std::string str;
	std::ifstream fin("saveCount.txt"/*, ios::binary*/);
	while (!fin.eof())
	{
		fin >> str;
		generated = "SavedGame" + str;
	}

	int temp = stoi(str);
	std::ofstream outfile("saveCount.txt");
	outfile << (temp + 1);
	outfile.close();

	return generated;
}
void Settings::SaveGame(Player currentPlayer, HWND consoleWindow)
{
	std::string settings = GetData();
	std::string playerInfo = currentPlayer.GetData();

	int saveGameBox = MessageBox(
		consoleWindow,
		(LPCWSTR)L"Save Game?",
		(LPCWSTR)L"Do you want to save game?",
		MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1
	);

	switch (saveGameBox)
	{
	case IDYES:
	{
		int newSaveBox = MessageBox(
			consoleWindow,
			(LPCWSTR)L"New Save?",
			(LPCWSTR)L"Do you want to create a new save?",
			MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1
		);

		switch (newSaveBox)
		{
		case IDYES:
		{
			std::string fileName = generate() + ".txt";

			std::ofstream outfile(fileName);
			outfile << settings;
			outfile << playerInfo;
			outfile.close();

			int savedNotification = MessageBox(
				consoleWindow,
				(LPCWSTR)L"SAVED",
				(LPCWSTR)L"Saved game.",
				MB_ICONQUESTION | MB_OK | MB_DEFBUTTON1
			);

			break;
		}
		case IDNO:
			std::string str;
			std::ifstream fin("saveCount.txt"/*, ios::binary*/);
			while (!fin.eof())
			{
				fin >> str;
			}

			std::string fileName = "SavedGame" + str;

			std::ofstream outfile(fileName);
			outfile << settings << std::endl;
			outfile << playerInfo << std::endl;
			outfile.close();

			int savedNotification = MessageBox(
				consoleWindow,
				(LPCWSTR)L"SAVED",
				(LPCWSTR)L"Saved game to last save.",
				MB_ICONQUESTION | MB_OK | MB_DEFBUTTON1
			);

			break;
		}
	}
	case IDNO:
		break;
	}
}