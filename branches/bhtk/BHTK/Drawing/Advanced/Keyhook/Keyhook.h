#pragma once

#include "../../Hook.h"

namespace Drawing {
	class Keyhook : public Hook {
		private:
			unsigned int* key;//Pointer to the current key
			std::wstring name;//Name of the hotkey
			unsigned int timeout;//Timeout to change hotkey if clicked
		public:
			//Two Hook Initializations; one for basic hooks, one for grouped hooks.
			EXPORT Keyhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int* key, std::wstring hotkeyName);
			EXPORT Keyhook(HookGroup* group, unsigned int x, unsigned int y, unsigned int* key, std::wstring hotkeyName);

			EXPORT std::wstring GetName() { return name; };
			EXPORT void SetName(std::wstring newName) { Lock(); name = newName; Unlock(); };

			EXPORT unsigned int GetKey() { return *key; };
			EXPORT void SetKey(unsigned int* newKey) { Lock(); key = newKey; Unlock(); };

			bool OnLeftClick(bool up, unsigned int x, unsigned int y);
			void OnDraw();
			bool OnKey(bool up, BYTE key, LPARAM lParam);

			EXPORT unsigned int GetXSize();
			EXPORT unsigned int GetYSize();
	};
};