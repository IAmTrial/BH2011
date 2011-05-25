#include "Texthook.h"
#include "../../../Common.h"
#include "../../../D2Ptrs.h"

using namespace std;
using namespace Drawing;

/* Basic Hook Initializer
 *		Used for just drawing basic text on screen.
 */
Texthook::Texthook(HookVisibility visibility, unsigned int x, unsigned int y, std::wstring formatString, ...) :
Hook(visibility, x, y) {
	//Correctly format the string from the given arguments.
	SetFont(0);
	SetColor(White);
	SetHoverColor(Disabled);
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
Texthook::Texthook(HookGroup *group, unsigned int x, unsigned int y, std::wstring formatString, ...) :
Hook(group, x, y) {
	//Correctly format the string from the given arguments.
	SetFont(0);
	SetColor(White);
	SetHoverColor(Disabled);
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
}

/* GetFont()
 *	Returns what d2 font we use to draw.
 */
unsigned int Texthook::GetFont() {
	return font;
}

/* SetFont(unsigned int newFont)
 *	Sets when the font to draw with.
 */
void Texthook::SetFont(unsigned int newFont) {
	Lock();
	font = newFont;
	Unlock();
}

/* GetColor()
 *	Returns what color the text will be drawn.
 */
TextColor Texthook::GetColor() {
	return color;
}

/* SetColor()
 *	Sets what color the text will be drawn in.
 */
void Texthook::SetColor(TextColor newColor) {
	Lock();
	color = newColor;
	Unlock();
}

/* GetHoverColor()
 *	Return what color the text will be when hovered.
 */
TextColor Texthook::GetHoverColor() {
	return hoverColor;
}

/* SetHoverColor()
 *	Sets what color to draw when hovered.
 */
void Texthook::SetHoverColor(TextColor newHoverColor) {
	Lock();
	hoverColor = newHoverColor;
	Unlock();
}

/* GetText()
 *	Returns what text will be drawn.
 */
std::wstring Texthook::GetText() {
	return text;
}

/* SetText(string formaString, ...)
 *	Sets a new formatted string as the text
 */
void Texthook::SetText(std::wstring formatString, ...) {
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
}

/* GetXSize()
 *	Returns how long the text is.
 */
unsigned int Texthook::GetXSize() {
	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(font);
	D2WIN_GetTextWidthFileNo((wchar_t*)text.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	return width; 
}

/* GetXSize()
 *	Returns how tall the text is.
 */
unsigned int Texthook::GetYSize() {
	unsigned int nHeight[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12};
	return nHeight[font];
}

/* Draw()
 *	Draws the text, must be called inside a Draw Patch
 */
void Texthook::OnDraw() {
	if (!IsActive())
		return;

	Lock();

	unsigned int drawColor = color;
	if (InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY) && GetHoverColor() != Disabled)
		drawColor = hoverColor;

	DWORD oldFont = D2WIN_SetTextSize(font);
	D2WIN_DrawText(text.c_str(), GetX(), GetY() + GetYSize(), drawColor, 0);
	D2WIN_SetTextSize(oldFont);
	Unlock();
}

/* OnLeftClick(bool up, unsigned int x, unsigned int y)
 *	Check if the text hook has been clicked on.
 */
bool Texthook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y) && GetLeftClickHandler()) {
		bool block = false;
		Lock();
		block = GetLeftClickHandler()(up, this, GetLeftClickVoid());
		Unlock();
		return block;
	}
	return false;
}

/* OnRightClick(bool up, unsigned int x, unsigned int y)
 *	Check if the text hook has been clicked on.
 */
bool Texthook::OnRightClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y) && GetRightClickHandler()) {
		bool block = false;
		Lock();
		block = GetRightClickHandler()(up, this, GetRightClickVoid());
		Unlock();
		return block;
	}
	return false;
}

/* GetTextSize(string text, unsigned int font)
 *	Returns the dimensions of the given text!
 */
POINT Texthook::GetTextSize(wstring text, unsigned int font) {
	unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12};
	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(font);
	D2WIN_GetTextWidthFileNo((wchar_t*)text.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	POINT point = {width, height[font]};
	return point;
}

/* Draw(unsigned int x, unsigned int y, unsigned int font, TextColor color, std::string text, ...)
 *	Static function to wrap drawing text on screen, to be used for things that are rapidly changing(Monsters on map, etc..)
 */
bool Texthook::Draw(unsigned int x, unsigned int y, int align, unsigned int font, TextColor color, std::wstring text, ...) {
	//Convert all %s's into proper values.
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, text);
	vswprintf_s(buffer, 4096, text.c_str(), arg);
	va_end(arg);

	unsigned int properX = x;
	if (align == Center)
		x = x - (GetTextSize(buffer, font).x / 2);

	if (align == Right)
		x = x - GetTextSize(buffer, font).x;

	//Draw the text!
	unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12};
	DWORD size = D2WIN_SetTextSize(font);
	D2WIN_DrawText(buffer, x, y + height[font], color, 0);
	D2WIN_SetTextSize(size);

	return true;
}