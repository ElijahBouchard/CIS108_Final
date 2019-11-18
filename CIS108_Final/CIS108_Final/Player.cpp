#include "Player.h"
#include "Settings.h"
Player::Player()
{
	fPlayerX = 3.0f;
	fPlayerY = 3.0f;
	fPlayerA = 0.0f;

	iPlayerHealth = 100.0f;

	PlayerSpeed = 1.0f;

	OnPad = false;
}

Player::Player(std::string savedGame)
{
	fPlayerX;
	fPlayerY;
	fPlayerA;

	iPlayerHealth;

	PlayerSpeed = 1.0f;

	OnPad = false;

	std::string filename = "SavedGame" + savedGame + ".txt";
	std::ifstream infile(filename);

	std::string line;
	for (int i = 0; i < 7; i++)
	{
		std::getline(infile, line);
		if (i == 3)
		{
			fPlayerX = stod(line);
		}
		else if (i == 4)
		{
			fPlayerY = stod(line);
		}
		else if (i == 5)
		{
			fPlayerA = stod(line);
		}
		else if (i == 6)
		{
			iPlayerHealth = stod(line);
		}
	}
}

void Player::Move(unsigned short key, float time, Settings currentSettings, HWND consoleWindow)
{
	if(key == (unsigned short)'A')
	{
		fPlayerA -= currentSettings.Sensitivity * time;
	}
	if (key == (unsigned short)'D')
	{
		fPlayerA += currentSettings.Sensitivity * time;
	}
	if (key == (unsigned short)'W')
	{
		fPlayerX += sinf(fPlayerA) * 5.0f * time;
		fPlayerY += cosf(fPlayerA) * 5.0f * time;

		//Converts and checks player position -> integer space
		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * time;
			fPlayerY -= cosf(fPlayerA) * 5.0f * time;
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '|'  && OnPad)
		{
			
		}
		else if(currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '|')
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * time;
			fPlayerY -= cosf(fPlayerA) * 5.0f * time;
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == 'X')
		{
			TakeDamage(0.1, consoleWindow);
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == 'K')
		{
			OnPad = true;
		}
	}
	if (key == (unsigned short)'S')
	{
		fPlayerX -= sinf(fPlayerA) * 5.0f * time;
		fPlayerY -= cosf(fPlayerA) * 5.0f * time;

		//Converts and checks player position -> integer space
		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * time;
			fPlayerY += cosf(fPlayerA) * 5.0f * time;
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '|'  && OnPad)
		{
		
		}
		else if(currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == '|')
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * time;
			fPlayerY += cosf(fPlayerA) * 5.0f * time;
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == 'X')
		{
			TakeDamage(0.1, consoleWindow);
		}

		if (currentSettings.map[(int)fPlayerY * currentSettings.cMapWidth + (int)fPlayerX] == 'K')
		{
			OnPad = true;
		}
	}
}
void Player::TakeDamage(float damage, HWND consoleWindow)
{
	if (iPlayerHealth <= 0)
	{
		int deathBox = MessageBox(
			consoleWindow,
			(LPCWSTR)L"You Have Died",
			(LPCWSTR)L"Exiting Game.",
			MB_ICONEXCLAMATION | MB_OK
		);

		exit(EXIT_SUCCESS);
	}
	else
	{
		iPlayerHealth -= damage;
	}
}
void Player::ResetPad()
{
	OnPad = false;
}
std::string Player::GetData()
{
	std::string data =
		std::to_string(fPlayerX) + "\n" +
		std::to_string(fPlayerY) + "\n" +
		std::to_string(fPlayerA) + "\n" +
		std::to_string(iPlayerHealth);

	return data;
}