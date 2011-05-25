#pragma once
#include <vector>
#include "../../Hook.h"

namespace Drawing {
	class Combohook : public Hook {
		private:
			std::vector<std::wstring> options;
			unsigned int xSize;
			unsigned int font;
			unsigned int* currentIndex;
			bool active;
		public:
			EXPORT Combohook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int xSize, unsigned int* currentIndex, std::vector<std::wstring> options);
			EXPORT Combohook(HookGroup* group, unsigned int x, unsigned int y, unsigned int xSize, unsigned int* currentIndex, std::vector<std::wstring> options);

			EXPORT std::vector<std::wstring> GetOptions() { return options; };
			EXPORT unsigned int NewOption(std::wstring opt) { Lock(); options.push_back(opt); Unlock(); return options.size() - 1; };
			EXPORT unsigned int GetSelectedIndex() { return *currentIndex; };
			EXPORT void SetSelectedIndex(unsigned int index) { if (index > options.size()) { return; } Lock(); *currentIndex = index; Unlock(); };

			EXPORT unsigned int GetFont() { return font; };
			EXPORT void SetFont(unsigned int newFont) { Lock(); font = newFont; Unlock(); };

			EXPORT unsigned int GetXSize() { return xSize; };
			EXPORT void SetXSize(unsigned int size) { Lock(); xSize = size; Unlock(); };

			EXPORT unsigned int GetYSize() { unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12}; return height[GetFont()]; };

			bool OnLeftClick(bool up, unsigned int x, unsigned int y);
			void OnDraw();

			EXPORT bool InHook(unsigned int nx, unsigned int ny) { return nx >= GetX() && ny >= GetY() && nx <= GetX() + GetXSize() + 5 && ny <= GetX() + GetYSize() + 3; };
	};
};