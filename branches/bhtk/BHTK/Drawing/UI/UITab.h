#pragma once
#include "../Hook.h"
#include "UI.h"

namespace Drawing {
	#define TAB_HEIGHT 13

	class UITab : public HookGroup {
		private:
			std::wstring name;
			UI* ui;
		public:
			EXPORT UITab(std::wstring name, UI* nui) : name(name), ui(nui) {ui->Tabs.push_back(this); if (ui->Tabs.size() == 1) { ui->SetCurrentTab(this); }};
			EXPORT ~UITab();

			EXPORT unsigned int GetX() { return ui->GetX(); };
			EXPORT unsigned int GetY() { return ui->GetY() + TITLE_BAR_HEIGHT + TAB_HEIGHT; };
			EXPORT unsigned int GetXSize() { return ui->GetXSize(); };
			EXPORT unsigned int GetYSize() { return ui->GetYSize() - TITLE_BAR_HEIGHT - TAB_HEIGHT; };

			EXPORT unsigned int GetTabPos();
			EXPORT unsigned int GetTabSize() { return (ui->GetXSize() / ui->Tabs.size()); };
			EXPORT unsigned int GetTabX() { return ui->GetX() + GetTabPos() * GetTabSize(); };
			EXPORT unsigned int GetTabY() { return ui->GetY() + TITLE_BAR_HEIGHT; };


			EXPORT bool IsActive() { return ui->GetActiveTab() == this && !ui->IsMinimized(); };

			EXPORT bool IsHovering(unsigned int x, unsigned int y) { return x >= GetTabX() && y >= GetTabY() && x <= (GetTabX() + GetTabSize()) && y <= (GetTabY() + TAB_HEIGHT); };

			void OnDraw();
	};
};