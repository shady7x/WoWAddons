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

	if (mode == 5)
	{
		int sl = 100, sr = 33;
		while (true) // rogue 5 assa recode
		{
			bool cc = GetAsyncKeyState(0x43);
			if (GetAsyncKeyState(0x5) || cc)
			{
				_color = GetPixel(_hdc, 1160, 0);
				int rbits = GetRValue(_color);
				int gbits = GetGValue(_color);
				int bbits = GetBValue(_color);

				bool melee = IsBitSet(rbits, 7);
				int cp = 0;
				if (IsBitSet(bbits, 0)) cp += 1;
				if (IsBitSet(bbits, 1)) cp += 2;
				if (IsBitSet(bbits, 2)) cp += 4;

				if (IsBitSet(gbits, 2)) CastKey(0x34); // tkick
				if (IsBitSet(gbits, 3)) CastKey(0x70); // kick1
				if (IsBitSet(gbits, 4)) CastKey(0x71); // kick2
				if (IsBitSet(gbits, 5)) CastKey(0x72); // kick3

				if (IsBitSet(gbits, 7)) CastKey(0x4A); // tea

				if (IsBitSet(bbits, 4)) CastKey(0x74); // mark for death

				cc = cc && IsBitSet(bbits, 3);

				bool skip = false;
				if (cc && IsBitSet(bbits, 5))
				{
					CastKey(0x74); // mark for death
					Sleep(sl + rand() % sr);
					skip = true;
				}
				if (!skip)
				{
					if (melee)
					{
						if (!cc && cp > 1 && IsBitSet(rbits, 5) && !IsBitSet(rbits, 6))
						{
							CastKey(0x33);
							Sleep(sl + rand() % sr);
						}
						else
						{
							if (cp > 3 || (!cc && IsBitSet(gbits, 1)))
							{
								if (cc) // kidney
								{
									CastKey(0x49);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(rbits, 2)) // rapture
								{
									CastKey(0x55);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(bbits, 6)) // blood storm
								{
									CastKey(0xDB);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(rbits, 4)) // envenom
								{
									CastKey(0x4B);
									Sleep(sl + rand() % sr);
								}
							}
							else
							{
								if (IsBitSet(rbits, 0)) // garrote
								{
									CastKey(0x52);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(rbits, 1)) // bonespike
								{
									CastKey(0x46);
									Sleep(sl + rand() % sr);
								}
								else if (cp <= 1 && IsBitSet(gbits, 0)) // echoing, might increase cp up to <= 3
								{
									CastKey(0x4C);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(gbits, 6)) // shiv_reprimand_1_off
								{
									CastKey(0x73);
									Sleep(sl + rand() % sr);
								}
								else if (IsBitSet(rbits, 3)) // mutilate
								{
									CastKey(0x59);
									Sleep(sl + rand() % sr);
								}
							}
						}
					}
					else
					{
						if (!cc && cp > 0)
						{
							if (IsBitSet(rbits, 5))
							{
								CastKey(0x33);
								Sleep(sl + rand() % sr);
							}
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