#include "../../../D2Ptrs.h"
#include "../../../Common.h"
#include "Colorhook.h"
#include "../../Basic/Boxhook/Boxhook.h"
#include "../../Basic/Framehook/Framehook.h"
#include "../../Basic/Texthook/Texthook.h"
#include "../../Basic/Crosshook/Crosshook.h"

using namespace Drawing;

Colorhook* Colorhook::current;

/* Basic Hook Initializer
 *		Used for just drawing basics.
 */
Colorhook::Colorhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int* color, std::wstring formatString, ...) :
Hook(visibility, x, y) {
	//Correctly format the string from the given arguments.
	currentColor = color;
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Colorhook::Colorhook(HookGroup *group, unsigned int x, unsigned int y, unsigned int* color, std::wstring formatString, ...) :
Hook(group, x, y) {
	//Correctly format the string from the given arguments.
	currentColor = color;
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
}

void Colorhook::SetText(std::wstring newText) {
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, newText);
	vswprintf_s(buffer, 4096, newText.c_str(), arg);
	va_end(arg);
	Lock();
	text = buffer;
	Unlock();
}

void Colorhook::SetColor(unsigned int newColor) {
	if (newColor < 0 || newColor > 255)
		return;
	Lock();
	*currentColor = newColor;
	Unlock();
}

bool Colorhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (Colorhook::current == this && x >= 310 && y >= 205 && x <= 490 && y <= 385 && up) {
		SetColor(curColor);
		Colorhook::current = false;
		return true;
	} else if (InRange(x,y) && Colorhook::current == false) {
		if (up)
			Colorhook::current = this;
		return true;
	}
	return false;
}

bool Colorhook::OnRightClick(bool up, unsigned int x, unsigned int y) {
	if (Colorhook::current == this) {
		Colorhook::current = false;
		return true;
	}
	return false;
}

/* GetXSize()
 *	Returns how long the text is.
 */
unsigned int Colorhook::GetXSize() {
	return Texthook::GetTextSize(GetText(), 0).x; 
}

/* GetXSize()
 *	Returns how tall the text is.
 */
unsigned int Colorhook::GetYSize() {
	return 10;
}

void Colorhook::OnDraw() {
	Lock();
	if (Colorhook::current == this) {
		//Draw the shaded background
		Boxhook::Draw(0, 0, Hook::GetScreenWidth(), Hook::GetScreenHeight(), 0, BTOneHalf);
		//Draw the actual choose color box
		Framehook::Draw(300, 200, 200, 200, 0, BTNormal);
		//Draw title
		Texthook::Draw(360, 205, false, 0, White, L"Choose Color");
		int col = 1;
		int mX = (*p_D2CLIENT_MouseX);
		int mY = (*p_D2CLIENT_MouseY);
		for (int n = 0, row = 1; n <= 255; n++, row++) {
			if (row == 18) {
				col++;
				row = 0;
			}
			if (mX >= 310 + (row * 10) && mY >= 205 + (col * 10) && mX <= 320 + (row * 10) && mY <= 215 + (col * 10))
				curColor = n;
			D2GFX_DrawRectangle(310 + (row * 10), 205 + (col * 10), 320 + (row * 10), 215 + (col * 10), n, 5);
		}
		CHAR szLines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};
		for(unsigned int x = 0; x < 12; x++)
			D2GFX_DrawLine(372 + szLines[x][0], 368 + szLines[x][1], 372 + szLines[x+1][0], 368 + szLines[x+1][1], curColor, -1);
		Texthook::Draw(312, 383, false, 0, White, L"Left Click - Select");
		Texthook::Draw(385, 364, false, 0, White, L"Right Click - Close");
	} else {
		Texthook::Draw(GetX() + 13, GetY() + 10, InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY)?Tan:Gold, 0, White, GetText());
		Crosshook::Draw(GetX(), GetY() + 4, GetColor());
	}
	Unlock();
}
