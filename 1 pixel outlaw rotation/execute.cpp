#include <windows.h>
#include <iostream>
#include <string>

HWND game = NULL;

void CastKey(int opcode)
{
	SendMessageA(game, WM_KEYDOWN, opcode, 0);
	Sleep(10 + rand() % 8);
	SendMessageA(game, WM_KEYUP, opcode, 0);
}

__forceinline bool IsBitSet(int num, int bit)
{
	return (1 == ((num >> bit) & 1));
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	//cPicker();

	std::string str = st(">V") + st("YS") + st("K ") + st("VM") + st(" >") + st("HY") + st("JY") + st("HM") + st("[");
	game = FindWindowA(NULL, str.c_str());
	if (game == NULL)
	{
		std::cout << "\nNF" << std::endl;
		return -1;
	}

	SetProcessDPIAware();
	HDC _hdc = GetDC(0);
	COLORREF _color;

	while (!GetAsyncKeyState(VK_SPACE)) Sleep(1);

	std::cout << ".";

	Sleep(500);

	std::cout << "..";

	short mode;
	std::cin >> mode;
	
	//..

	if (mode == 2)
	{
		int sl = 70, sr = 33;
		while (true) // rogue 2
		{
			if (GetAsyncKeyState(0x5))
			{
				_color = GetPixel(_hdc, 966, 326);
				int cpoint = GetRValue(_color);
				int energy = GetGValue(_color);
				int cdbits = GetBValue(_color);

				if (cpoint > 10 || energy > 200)
					continue;

				bool inMelee = IsBitSet(cdbits, 0);
				bool bSinister = IsBitSet(cdbits, 1); // basically energy >= 45 or ?
				bool bAmbush = IsBitSet(cdbits, 2);   // ambush can only be used from invis and energy >= 50
				bool bDice = IsBitSet(cdbits, 3);     // dice 45 sec cd and energy >= 25
				bool bEyes = IsBitSet(cdbits, 4);     // eyes 45 sec cd and energy >= 25
				bool bSnd = IsBitSet(cdbits, 5);      // snd less < 6 sec uptime and energy >= 25
				bool bPistol = IsBitSet(cdbits, 6);   // proc up and energy >= 40
				bool bBlade = IsBitSet(cdbits, 7);    // blade 30 sec cd and energy >= 15

				if (inMelee)
				{
					if (bDice)
					{
						CastKey(0x59);
						Sleep(sl + rand() % sr);
					}
					else if (bBlade)
					{
						CastKey(0x47);
						Sleep(sl + rand() % sr);
					}
					else if (bEyes && cpoint >= 5)
					{
						CastKey(0x52);
						Sleep(sl + rand() % sr);
					}
					else if (bSnd && cpoint >= 5)
					{
						CastKey(0x33);
						Sleep(sl + rand() % sr);
					}
					else if (bPistol && cpoint < 5)
					{
						CastKey(0x46);
						Sleep(sl + rand() % sr);
					}
					else if (!bAmbush && cpoint < 5)
					{
						CastKey(0x31);
						Sleep(sl + rand() % sr);
					}
					else if (energy > 30) // >= 35
					{
						CastKey(0x32);
						Sleep(sl + rand() % sr);
					}
				}

				Sleep(1);
			}
		}
	}
}

/*

r     - arena1
g     - arena2
b     - arena3
r,g   - arena1,2
r,b   - arena1,3
g,b   - arena2,3
r,g,b - arena1,2,3

*/