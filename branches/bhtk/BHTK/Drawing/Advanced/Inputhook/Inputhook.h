#pragma once

#include <string>
#include "../../Hook.h"
#include "../../Basic/Texthook/Texthook.h"

namespace Drawing {
	class Inputhook : public Hook {
		private:
			std::wstring text; //Text that is actually in the input box
			bool active, showCursor; //Booleans set if the hook is active / currently showing cursor.
			unsigned int xSize; //Length of the input box
			unsigned int cursorPos, cursorTick; //Cursor Position / Timer to control cursor blink
			unsigned int textPos;//Used to determine which part of the current text I should show
			unsigned int selectPos, selectLength; // Selection position and length
			unsigned int font; //What type of font to use in the input hook.
		public:
			EXPORT Inputhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int xSize, std::wstring formatString, ...);
			EXPORT Inputhook(HookGroup* group, unsigned int x, unsigned int y, unsigned int xSize, std::wstring formatString, ...);

			//Getters and Setters

			//Text in the input box
			EXPORT std::wstring GetText() { return text; };
			EXPORT void SetText(std::wstring newText, ...);

			//If the inputhook box is active(Can be typed in)
			EXPORT bool IsActive() { return active; };
			EXPORT void SetActive(bool isActive) { Lock(); active = isActive; Unlock(); };

			//Font Size
			EXPORT unsigned int GetFont() { return font; };
			EXPORT void SetFont(unsigned int newFont);

			//X Size
			EXPORT unsigned int GetXSize() { return xSize; };
			EXPORT void SetXSize(unsigned int newXSize);

			//Y Size
			EXPORT unsigned int GetYSize() { unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12}; return height[GetFont()]; };

			//If we are current showing the cursor, for blinking purposes!
			EXPORT bool ShowCursor() { return showCursor; };
			EXPORT void SetCursorState(bool state) { Lock(); showCursor = state; Unlock(); };
			EXPORT void ToggleCursor() { SetCursorState(!ShowCursor()); };

			EXPORT void CursorTick();
			EXPORT void ResetCursorTick() { cursorTick = 0; };

			EXPORT unsigned int GetCursorPosition() { return cursorPos; };
			EXPORT void SetCursorPosition(unsigned int newPosition);
			EXPORT void IncreaseCursorPosition(unsigned int len);
			EXPORT void DecreaseCursorPosition(unsigned int len);

			EXPORT unsigned int GetSelectionPosition() { return selectPos; };
			EXPORT void SetSelectionPosition(unsigned int pos);

			EXPORT unsigned int GetSelectionLength() { return selectLength; };
			EXPORT void SetSelectionLength(unsigned int length);

			EXPORT bool IsSelected() { return selectLength > 0; };
			EXPORT void ResetSelection() { Lock(); selectPos = 0; selectLength = 0; Unlock(); };
		
			bool OnKey(bool up, BYTE key, LPARAM lParam);
			bool OnLeftClick(bool up, unsigned int x, unsigned int y);
			bool OnRightClick(bool up, unsigned int x, unsigned int y);

			EXPORT unsigned int GetCharacterLimit();

			EXPORT unsigned int GetTextPos() { return textPos; };
			EXPORT void SetTextPos(unsigned int pos) { Lock(); textPos = pos; Unlock(); };

			void OnDraw();

			EXPORT void InputText(std::wstring newText);
			EXPORT void Backspace();
			EXPORT void Replace(unsigned int pos, unsigned int len, std::wstring str);
			EXPORT void Erase(unsigned int pos, unsigned int len);
	};
};