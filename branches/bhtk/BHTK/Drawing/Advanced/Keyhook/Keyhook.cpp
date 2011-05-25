#include "Keyhook.h"
#include "../../../D2Ptrs.h"
#include "../../../Common.h"

using namespace std;
using namespace Drawing;

/* Basic Hook Initializer
 *		Used for just drawing basics.
 */
Keyhook::Keyhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int* key, std::wstring hotkeyName) :
Hook(visibility, x, y) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
	SetName(hotkeyName);
}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Keyhook::Keyhook(HookGroup *group, unsigned int x, unsigned int y, unsigned int* key, std::wstring hotkeyName) :
Hook(group, x, y) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
	SetName(hotkeyName);
}

bool Keyhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y)) {
		if (up) {
			if (!timeout)
				timeout = GetTickCount();
			else
				timeout = 0;
		}
		return true;
	}
	return false;
}

void Keyhook::OnDraw() {
	KeyCode keyCode = GetKeyCode(GetKey());
	wstring prefix = L"";
	if (name.length() > 0)
		prefix = name + L":ÿc4 ";

	wstring text = prefix + keyCode.literalName;
	if (timeout) {
		double time = 3 - floor((double)(GetTickCount() - timeout) / 1000);
		if (time <= 0)
			timeout = 0;
		wchar_t num[100];
		_itow_s((int)time, num, 100, 10);
		text = prefix + wstring(num) + L" secs";
	}
	DWORD size = D2WIN_SetTextSize(0);
	D2WIN_DrawText(text.c_str(), GetX(), GetY() + 10, InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY)?7:4, 0);
	D2WIN_SetTextSize(size);
}

bool Keyhook::OnKey(bool up, BYTE kkey, LPARAM lParam) {
	if (timeout) {
		Lock();
		if (up) {
			if (kkey == VK_ESCAPE)
				kkey = 0;
			*key = (unsigned int)kkey;
			timeout = 0;
		}
		Unlock();
		return true;
	}
	return false;
}

unsigned int Keyhook::GetXSize() {
	KeyCode keyCode = GetKeyCode(GetKey());
	wstring prefix = L"";
	if (name.length() > 0)
		prefix = name + L":ÿc4 ";
	wstring text = prefix + keyCode.literalName;
	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(0);
	D2WIN_GetTextWidthFileNo((wchar_t*)text.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	return width; 
}

unsigned int Keyhook::GetYSize() {
	return 10;
}