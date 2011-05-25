#pragma once

#include "../../Hook.h"
#include <string>

namespace Drawing {
	enum TextColor {Disabled=-1,White=0,Red,Green,Blue,Gold,Grey,Black,Tan,Orange,Yellow,DarkGreen,Purple,Silver=15};

	class Texthook : public Hook {
		private:
			std::wstring text;//Text to be drawn.
			unsigned int font;//Determines which built-in diablo font to use.
			TextColor color, hoverColor;//Determines which color to use.
		public:
			//Two Hook Initializations; one for basic hooks, one for grouped hooks.
			EXPORT Texthook(HookVisibility visibility, unsigned int x, unsigned int y, std::wstring formatString, ...);
			EXPORT Texthook(HookGroup* group, unsigned int x, unsigned int y, std::wstring formatString, ...);

			//Returns what D2 Font we are drawing with.
			EXPORT unsigned int GetFont();

			//Sets what D2 Font to draw with
			EXPORT void SetFont(unsigned int newFont);

		
			//Returns what D2 Color we are drawing with.
			EXPORT TextColor GetColor();

			//Sets what D2 Color we are drawing with.
			EXPORT void SetColor(TextColor newColor);


			//Returns what D2 Color we want when mouse is hovering over text, default is disabled.
			EXPORT TextColor GetHoverColor();

			//Set what D2 Color we should draw with when hovered.
			EXPORT void SetHoverColor(TextColor newHoverColor);


			//Returns what text we are drawing
			EXPORT std::wstring GetText();

			//Set what text you want drawn
			EXPORT void SetText(std::wstring formatString, ...);

			//Determine the pixel length of the text.
			EXPORT unsigned int GetXSize();

			//Determine the pixel height of the text.
			EXPORT unsigned int GetYSize();

			//Draw the text.
			void OnDraw();

			//Checks if we've been clicked on and calls the handler if so.
			bool OnLeftClick(bool up, unsigned int x, unsigned int y);

			//Checks if we've been clicked on and calls the handler if so.
			bool OnRightClick(bool up, unsigned int x, unsigned int y);

			//Handy function to have!
			EXPORT static POINT GetTextSize(std::wstring text, unsigned int font);

			//Static draw text function
			EXPORT static bool Draw(unsigned int x, unsigned int y, int align, unsigned int font, TextColor color, std::wstring text, ...);
	};
};