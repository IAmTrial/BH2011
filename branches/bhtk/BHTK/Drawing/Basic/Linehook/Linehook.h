#pragma once

#include "../../Hook.h"

namespace Drawing {
	class Linehook : public Hook {
		private:
			unsigned int color;//Color of the line hook 0-255.
			unsigned int x2, y2;//2 of the line hook.

		public:
			//Linehook Initaliztors, one for basic hooks and one for groups.
			EXPORT Linehook(HookVisibility visiblity, unsigned int x, unsigned int y, unsigned int x2, unsigned int y2);
			EXPORT Linehook(HookGroup* group, unsigned int x, unsigned int y, unsigned int x2, unsigned int y2);

			//Returns the color of the line hook.
			EXPORT unsigned int GetColor();

			//Sets the color of the line hook.
			EXPORT void SetColor(unsigned int newColor);


			//Get the size of the line hook.
			EXPORT unsigned int GetX2();

			//Set the size of the x hook.
			EXPORT void SetX2(unsigned int newX);

			//Filler to return nothing to base class.
			EXPORT unsigned int GetXSize();
			EXPORT unsigned int GetYSize();

			//Get the height of the line hook.
			EXPORT unsigned int GetY2();

			//Set the height of the line hook.
			EXPORT void SetY2(unsigned int newY);

			//Draw the text.
			void OnDraw();

			//Static line draw
			EXPORT static bool Draw(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned int color);
	};
};