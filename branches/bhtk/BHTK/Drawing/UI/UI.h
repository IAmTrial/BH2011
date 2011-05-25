#pragma once

#include <Windows.h>
#include <string>
#include <list>
#include "../Hook.h"

namespace Drawing {
	class UI;
	class UITab;

	#define TITLE_BAR_HEIGHT 15
	#define MINIMIZED_Y_POS 460
	#define MINIMIZED_X_POS 10

	class UI : public HookGroup {
		private:
			static std::list<UI*> UIs;
			static std::list<UI*> Minimized;
			unsigned int x, y, xSize, ySize, zOrder;//Position and Size and Order
			bool active, minimized, dragged;//If UI is active or minimized or dragged
			unsigned int dragX, dragY;//Position where we grabbed it.
			std::wstring name;//Name of the UI
			UITab* currentTab;//Current tab open at the time.
			CRITICAL_SECTION crit;//Critical section
		public:
			std::list<UITab*> Tabs;

			EXPORT UI(std::wstring name, unsigned int xSize, unsigned int ySize);
			EXPORT ~UI();

			EXPORT void Lock() { EnterCriticalSection(&crit); };
			EXPORT void Unlock() { LeaveCriticalSection(&crit); };

			EXPORT unsigned int GetX() { return x; };
			EXPORT unsigned int GetY() { return y; };
			EXPORT unsigned int GetXSize() { return xSize; };
			EXPORT unsigned int GetYSize() { return ySize; };
			EXPORT bool IsActive() { return active; };
			EXPORT bool IsMinimized() { return minimized; };
			EXPORT bool IsDragged() { return dragged; };
			EXPORT std::wstring GetName() { return name; };
			EXPORT unsigned int GetZOrder() { return zOrder; };

			EXPORT void SetX(unsigned int newX);
			EXPORT void SetY(unsigned int newY);
			EXPORT void SetXSize(unsigned int newXSize);
			EXPORT void SetYSize(unsigned int newYSize);
			EXPORT void SetActive(bool newState) { Lock(); active = newState; Unlock(); };
			EXPORT void SetMinimized(bool newState);
			EXPORT void SetName(std::wstring newName) { Lock(); name = newName;  Unlock(); };
			EXPORT void SetDragged(bool state);
			EXPORT void SetZOrder(unsigned int newZ) { Lock(); zOrder = newZ; Unlock(); };

			EXPORT UITab* GetActiveTab() { if (!currentTab) { currentTab = (*Tabs.begin()); } return currentTab; };
			EXPORT void SetCurrentTab(UITab* tab) { Lock(); currentTab = tab; Unlock(); };

			void OnDraw();
			static void Draw();

			EXPORT static void Sort(UI* zero);

			bool OnLeftClick(bool up, unsigned int mouseX, unsigned int mouseY);
			static bool LeftClick(bool up, unsigned int mouseX, unsigned int mouseY);

			bool OnRightClick(bool up, unsigned int mouseX, unsigned int mouseY);
			static bool RightClick(bool up, unsigned int mouseX, unsigned int mouseY);

			EXPORT bool InWindow(unsigned int xPos, unsigned int yPos) { return xPos >= x && xPos <= x + xSize && yPos >= y && yPos <= y + ySize; };
			EXPORT bool InTitle(unsigned int xPos, unsigned int yPos) { return xPos >= x && xPos <= x + xSize && yPos >= y && yPos <= y + TITLE_BAR_HEIGHT; };
			EXPORT static bool InPos(unsigned int xPos, unsigned int yPos, unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize) { return xPos >= x && xPos <= x + xSize && yPos >= y && yPos <= y + ySize; };
	};
};