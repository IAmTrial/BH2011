#include <string>
#include <vector>
#include <Windows.h>
#include "Common.h"

using namespace std;

void Tokenize(const wstring& str,
                      vector<wstring>& tokens,
                      const wstring& delimiters = L" ")
{
    // Skip delimiters at beginning.
    wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    wstring::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
	return buf;
}

char* UnicodeToAnsi(const wchar_t* str)
{
	char* buf = NULL;
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, buf, 0, "?", NULL);
	buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, str, -1, buf, len, "?", NULL);
	return buf;
}

std::wstring Trim(std::wstring source) {
	source = source.erase(0, source.find_first_not_of(L" "));
	source = source.erase(source.find_last_not_of(L" ") + 1);
	source = source.erase(0, source.find_first_not_of(L"\t"));
	source = source.erase(source.find_last_not_of(L"\t") + 1);
	return source;
}

bool StringToBool(std::wstring str) {
	if ((_wcsicmp(str.c_str(), L"1") == 0) || (_wcsicmp(str.c_str(), L"y") == 0) || (_wcsicmp(str.c_str(), L"yes") == 0) || (_wcsicmp(str.c_str(), L"true") == 0))
		return true;
	return false;
}

KeyCode pCodes[] = {
	{L"Unknown", 0, L"Not Set"},
	{L"VK_BACK", 0x08, L"Backspace"},
	{L"VK_TAB", 0x09, L"Tab"},
	{L"VK_CLEAR", 0x0C, L"Clear"},
	{L"VK_RETURN", 0x0D, L"Enter"},
	{L"VK_SHIFT", 0x10, L"Shift"},
	{L"VK_CONTROL", 0x11, L"Control"},
	{L"VK_ALT", 0x12, L"Alt"},
	{L"VK_PAUSE", 0x13, L"Pause"},
	{L"VK_CAPSLOCK", 0x14, L"Caps Lock"},
	{L"VK_ESCAPE", 0x1B, L"Esc"},
	{L"VK_SPACE", 0x20, L"Space"},
	{L"VK_PAGEUP", 0x21, L"Page Up"},
	{L"VK_PAGEDN", 0x22, L"Page Down"},
	{L"VK_END", 0x23, L"End"},
	{L"VK_HOME", 0x24, L"Home"},
	{L"VK_LEFT", 0x25, L"Left"},
	{L"VK_UP", 0x26, L"Up"},
	{L"VK_RIGHT", 0x27, L"Right"},
	{L"VK_DOWN", 0x28, L"Down"},
	{L"VK_SELECT", 0x29, L"Select"},
	{L"VK_PRINT", 0x2A, L"Print"},
	{L"VK_EXECUTE", 0x2B, L"Execute"},
	{L"VK_SNAPSHOT", 0x2C, L"Prt Scr"},
	{L"VK_INSERT", 0x2D, L"Insert"},
	{L"VK_DELETE", 0x2E, L"Delete"},
	{L"VK_HELP", 0x2F, L"Help"},
	{L"VK_0", 0x30, L"0"}, {L"VK_1", 0x31, L"1"}, {L"VK_2", 0x32, L"2"}, {L"VK_3", 0x33, L"3"},
	{L"VK_4", 0x34, L"4"}, {L"VK_5", 0x35, L"5"}, {L"VK_6", 0x36, L"6"}, {L"VK_7", 0x37, L"7"},
	{L"VK_8", 0x38, L"8"}, {L"VK_9", 0x39, L"9"},
	{L"VK_A", 0x41, L"A"}, {L"VK_B", 0x42, L"B"}, {L"VK_C", 0x43, L"C"}, {L"VK_D", 0x44, L"D"},
	{L"VK_E", 0x45, L"E"}, {L"VK_F", 0x46, L"F"}, {L"VK_G", 0x47, L"G"}, {L"VK_H", 0x48, L"H"},
	{L"VK_I", 0x49, L"I"}, {L"VK_J", 0x4A, L"J"}, {L"VK_K", 0x4B, L"K"}, {L"VK_L", 0x4C, L"L"},
	{L"VK_M", 0x4D, L"M"}, {L"VK_N", 0x4E, L"N"}, {L"VK_O", 0x4F, L"O"}, {L"VK_P", 0x50, L"P"},
	{L"VK_Q", 0x51, L"Q"}, {L"VK_R", 0x52, L"R"}, {L"VK_S", 0x53, L"S"}, {L"VK_T", 0x54, L"T"},
	{L"VK_U", 0x55, L"U"}, {L"VK_V", 0x56, L"V"}, {L"VK_W", 0x57, L"W"}, {L"VK_X", 0x58, L"X"},
	{L"VK_Y", 0x59, L"Y"}, {L"VK_Z", 0x5A, L"Z"},
	{L"VK_NUMPAD0", 0x60, L"Numpad 0"}, {L"VK_NUMPAD1", 0x61, L"Numpad 1"},
	{L"VK_NUMPAD2", 0x62, L"Numpad 2"}, {L"VK_NUMPAD3", 0x63, L"Numpad 3"},
	{L"VK_NUMPAD4", 0x64, L"Numpad 4"}, {L"VK_NUMPAD5", 0x65, L"Numpad 5"},
	{L"VK_NUMPAD6", 0x66, L"Numpad 6"}, {L"VK_NUMPAD7", 0x67, L"Numpad 7"},
	{L"VK_NUMPAD8", 0x68, L"Numpad 8"}, {L"VK_NUMPAD9", 0x69, L"Numpad 9"},
	{L"VK_NUMPADMULTIPLY", 0x6A, L"Numpad *"}, {L"VK_NUMPADADD", 0x6B, L"Numpad +"},
	{L"VK_NUMPADSUBTRACT", 0x6D, L"Numpad -"}, {L"VK_NUMPADDECIMAL", 0x6E, L"Numpad ."}, 
	{L"VK_NUMPADDIVIDE", 0x6F, L"Numpad /"},
	{L"VK_F1", 0x70, L"F1"}, {L"VK_F2", 0x71, L"F2"}, {L"VK_F3", 0x72, L"F3"}, {L"VK_F4", 0x73, L"F4"},
	{L"VK_F5", 0x74, L"F5"}, {L"VK_F6", 0x75, L"F6"}, {L"VK_F7", 0x76, L"F7"}, {L"VK_F8", 0x77, L"F8"},
	{L"VK_F9", 0x78, L"F9"}, {L"VK_F10", 0x79, L"F10"}, {L"VK_F11", 0x7A, L"F11"}, {L"VK_F12", 0x7B, L"F12"},
	{L"VK_F13", 0x7C, L"F13"}, {L"VK_F14", 0x7D, L"F14"}, {L"VK_F15", 0x7E, L"F15"}, {L"VK_F16", 0x7F, L"F16"},
	{L"VK_NUMLOCK", 0x90, L"Numlock"}, {L"VK_SCROLL", 0x91, L"Scrolllock"}, {L"VK_LSHIFT", 0xA0, L"Left Shift"},
	{L"VK_RSHIFT", 0xA1, L"Right Shift"}, {L"VK_LCTRL", 0xA2, L"Left Ctrl"}, {L"VK_RCTRL", 0xA3, L"Right Ctrl"},
	{L"VK_LMENU", 0xA4, L"Left Menu"}, {L"VK_RMENU", 0xA5, L"Right Menu"}, 
	{L"VK_SEMICOLON", 0xBA, L";"}, {L"VK_PLUS", 0xBB, L"+"}, {L"VK_COMMA", 0xBC, L","}, {L"VK_MINUS", 0xBD, L"-"},
	{L"VK_PERIOD", 0xBE, L"."}, {L"VK_FORWARDSLASH", 0xBD, L"/"}, {L"VK_TILDE", 0xBF, L"~"},
	{L"VK_LEFTBRACKET", 0xDB, L"["}, {L"VK_BACKSLASH", 0xDC, L"\\"}, {L"VK_RIGHTBRACKET", 0xDD, L"]"},
	{L"VK_QUOTE", 0xDE, L"'"}};

KeyCode GetKeyCode(unsigned int nKey) {
	for (int n = 1; n < (sizeof(pCodes) / sizeof(pCodes[0])); n++)
		if (nKey == pCodes[n].value)
			return pCodes[n];
	return pCodes[0];
}
KeyCode GetKeyCode(const wchar_t* name) {
	for (int n = 1; n < (sizeof(pCodes) / sizeof(pCodes[0])); n++)
		if (!_wcsicmp(name, pCodes[n].name.c_str()))
			return pCodes[n];
	return pCodes[0];
}