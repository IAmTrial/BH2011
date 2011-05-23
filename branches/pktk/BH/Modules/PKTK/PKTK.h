#pragma once

#include "../Module.h"
#include "../../Config.h"
#include "../../Drawing.h"

struct UnitAny;

class PKTK : public Module {
	private:
		static map<std::string, Toggle> Toggles;

		Drawing::UITab* settingsTab;

	public:

		PKTK() : Module("PKTK") {};

		void OnLoad();
		void OnUnload();

		void OnLoop();
		void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
		void OnLeftClick(bool up, int x, int y, bool* block);
		void OnGameJoin(const string& name, const string& pass, int diff);
		void OnGameExit();
		void OnDraw();
		void OnGamePacketRecv(BYTE* packet, bool* block);
};
