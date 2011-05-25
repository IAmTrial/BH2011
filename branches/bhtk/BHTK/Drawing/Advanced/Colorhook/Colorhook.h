#pragma once

#include "../../Hook.h"

namespace Drawing {
	class Colorhook;

	class Colorhook : public Hook {
		private:
			std::wstring text;//Text to have linked
			unsigned int* currentColor;//Color that we will be changing
			unsigned int curColor;
		public:
			static Colorhook* current;//Pointer to the current colorhook

			//Two Hook Initializations; one for basic hooks, one for grouped hooks.
			EXPORT Colorhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int* color, std::wstring formatString, ...);
			EXPORT Colorhook(HookGroup* group, unsigned int x, unsigned int y, unsigned int* color, std::wstring formatString, ...);

			EXPORT std::wstring GetText() { return text; };
			EXPORT void SetText(std::wstring newText);

			EXPORT unsigned int GetColor() { return *currentColor; };
			EXPORT void SetColor(unsigned int newColor);

			bool OnLeftClick(bool up, unsigned int x, unsigned int y);
			bool OnRightClick(bool up, unsigned int x, unsigned int y);
			void OnDraw();

			EXPORT unsigned int GetXSize();
			EXPORT unsigned int GetYSize();
	};
};