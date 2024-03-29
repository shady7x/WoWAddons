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

	if (mode == 3)
	{
		int sl = 100, sr = 33;
		while (true) // rogue 3
		{
			if (GetAsyncKeyState(0x5))
			{
				_color = GetPixel(_hdc, 1160, 0);
				int cp = GetRValue(_color);
				int cp_mx = 5;
				if (cp >= 20)
				{
					cp -= 20;
					cp_mx = 7;
				}
				else if (cp >= 10)
				{
					cp -= 10;
					cp_mx = 6;
				}

				if (cp > cp_mx)
					continue;

				bool cp_filled = (cp + 1 >= cp_mx);

				int cdbits = GetGValue(_color);

				if (IsBitSet(cdbits, 3)) // dice 45 sec cd and energy >= 25
				{
					CastKey(0x59);
					Sleep(sl + rand() % sr);
				}
				else if (IsBitSet(cdbits, 5) && cp > 4) // snd less < 6 sec uptime and energy >= 25
				{
					CastKey(0x33);
					Sleep(sl + rand() % sr);
				}
				else
				{
					int cdbits2 = GetBValue(_color);

					if (IsBitSet(cdbits2, 0)) // in melee range
					{
						if (IsBitSet(cdbits, 7) && !(IsBitSet(cdbits2, 1) || IsBitSet(cdbits2, 2))) // blade 30 sec cd and energy >= 15 and !(invis or vanish)
						{
							CastKey(0x47);
							Sleep(sl + rand() % sr);
						}
						else if (cp_filled)
						{
							if (IsBitSet(cdbits, 4)) // eyes 45 sec cd and energy >= 25
							{
								CastKey(0x52);
								Sleep(sl + rand() % sr);
							}
							else if (IsBitSet(cdbits, 0)) // dispatch usable and energy >= 35
							{
								CastKey(0x4A);
								Sleep(sl + rand() % sr);
							}
						} // cp not filled
						else if (IsBitSet(cdbits, 6)) // pistol proc up and energy >= 20
						{
							CastKey(0x46);
							Sleep(sl + rand() % sr);
						}
						else if (IsBitSet(cdbits2, 5) || // audacity
							IsBitSet(cdbits2, 4) || // shadowdance
							IsBitSet(cdbits2, 3) || // subterfuge
							IsBitSet(cdbits2, 2) || // vanish
							IsBitSet(cdbits2, 1))  // invis
						{
							if (IsBitSet(cdbits, 2)) // ambush usable (energy >= 50))
							{
								CastKey(0x55);
								Sleep(sl + rand() % sr);
							}
						}
						else if (IsBitSet(cdbits, 1)) // sinister strike usable (energy >= 45)
						{
							CastKey(0x31);
							Sleep(sl + rand() % sr);
						}
					}
				}
			}
			Sleep(1);
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