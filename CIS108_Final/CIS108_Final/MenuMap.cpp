#include "MenuMap.h"

std::string menuMap1[10] =
{
"",
"",
"",
"           >Start Game<",
"            How to Play",
"             Controls",
"",
"",
"",
"(Use W, and S to change selection)"
};
std::string menuMap2[10] =
{
"",
"",
"",
"            Start Game",
"           >How to Play<",
"             Controls",
"",
"",
"",
"(Use W, and S to change selection)"
};
std::string menuMap3[10] =
{
"",
"",
"",
"            Start Game",
"            How to Play",
"            >Controls<",
"",
"",
"",
"(Use W, and S to change selection)"
};

MenuMap::MenuMap()
{
	state = 0;
	currentSelection = "start game";

	PrintMap(menuMap1);
}
void MenuMap::PrintMap(std::string* menu)
{
	for (int i = 0; i < 10; i++)
	{
		if (menu[i] == "")
		{
			std::cout << std::endl;
		}
		else
		{
			std::cout << menu[i] << std::endl;
		}
	}
}
void MenuMap::MoveSelection(unsigned short key)
{
	if (key == 'S')
	{
		if (state == 2)
		{
			state = 0;
		}
		else
		{
			state++;
		}
	}
	if (key == 'W')
	{
		if (state == 0)
		{
			state = 2;
		}
		else
		{
			state--;
		}
	}

	if (state == 0)
	{
		system("CLS");
		PrintMap(menuMap1);
		currentSelection = "start game";
	}
	else if (state == 1)
	{
		system("CLS");
		PrintMap(menuMap2);
		currentSelection = "how to play";
	}
	else if (state == 2)
	{
		system("CLS");
		PrintMap(menuMap3);
		currentSelection = "controls";
	}
}
std::string MenuMap::Select()
{
	return currentSelection;
}
void MenuMap::ShowControls()
{
	std::string controls[10] =
	{
	"",
	"",
	"            W - Move Forward",
	"            A - Turn Left",
	"            S - Move Backward",
	"            D - Turn Right",
	"            O - Save Game",
	"            P - Quit Game",
	"",
	"                 >Back<"
	};

	system("CLS");
	PrintMap(controls);
	currentSelection = "back";
}
void MenuMap::ShowSettings(Settings settings, Player player, int selection)
{
	system("CLS");
	std::string settingsMap1[10] =
	{
	"",
	"                                             How to Play",
	"",
	"             The goal of the game is to navigate through the maze and step on the U-Pad.",
	"            It will then unlock the door and allow the player to move on to the next maze.",
	"   If the floor has a - symbol then it is a trap and the player will take damage when moving on it.",
	"       Some traps are unavoidable, however, and it is recommended to frequently save the game.",
	"",
	"                                              >Back<"
	};
	PrintMap(settingsMap1);
	currentSelection = "back";
}
void MenuMap::Back()
{
	system("CLS");
	for (int i = 0; i < 10; i++)
	{
		if (menuMap1[i] == "")
		{
			std::cout << std::endl;
		}
		else
		{
			std::cout << menuMap1[i] << std::endl;
		}
	}
	currentSelection = "start game";
	state = 0;
}
void MenuMap::ShowSaves(int num)
{
	std::string saves1[10] =
	{
	"",
	"",
	"                New Game",
	"               Load Save",
	"",
	"",
	"",
	"",
	"",
	"                 >Back<"
	};

	std::string saves2[10] =
	{
	"",
	"",
	"                New Game",
	"              >Load Save<",
	"",
	"",
	"",
	"",
	"",
	"                  Back"
	};

	std::string saves3[10] =
	{
	"",
	"",
	"               >New Game<",
	"               Load Save",
	"",
	"",
	"",
	"",
	"",
	"                  Back"
	};

	if (num == 0)
	{
		currentSelection = "back";
		system("CLS");
		PrintMap(saves1);
	}
	else if (num == 1)
	{
		currentSelection = "load save";
		system("CLS");
		PrintMap(saves2);
	}
	else 
	{
		currentSelection = "new game";
		system("CLS");
		PrintMap(saves3);
	}
}

void printToCoordinates(int x, int y, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	printf("\033[%d;%dH", x, y);
	vprintf(format, args);
	va_end(args);
	fflush(stdout);
}