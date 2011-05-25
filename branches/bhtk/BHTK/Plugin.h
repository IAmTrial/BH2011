#pragma once
#define EXPORTING
#include <string>
#include "Exports.h"
#include <Windows.h>

class Plugin {
	private:
		std::wstring name;
		std::wstring author;
		double version;
		bool loaded;
	public:
		EXPORT Plugin(std::wstring name, std::wstring author, double version);
		EXPORT ~Plugin();

		EXPORT std::wstring GetName() { return name; };
		EXPORT std::wstring GetAuthor() { return author; };
		EXPORT double GetVersion() { return version; };
		EXPORT bool IsLoaded() { return loaded; };

		void Load();
		void Unload();

		// Module Events
		EXPORT virtual void OnLoad() {};
		EXPORT virtual void OnUnload() {};

		EXPORT virtual void OnLoop() {};

		// Game Events
		//EXPORT virtual void OnGameJoin(const string& name, const string& pass, int diff) {}
		EXPORT virtual void OnGameExit() {};

		// Drawing Events
		EXPORT virtual void OnDraw() {};
		EXPORT virtual void OnAutomapDraw() {};
		EXPORT virtual void OnOOGDraw() {};

		EXPORT virtual void OnLeftClick(bool up, int x, int y, bool* block) {};
		EXPORT virtual void OnRightClick(bool up, int x, int y, bool* block) {};
		EXPORT virtual void OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {};

		EXPORT virtual void OnChatPacketRecv(BYTE* packet, bool* block) {};
		EXPORT virtual void OnRealmPacketRecv(BYTE* packet, bool* block) {};
		EXPORT virtual void OnGamePacketRecv(BYTE* packet, bool* block) {};
};
