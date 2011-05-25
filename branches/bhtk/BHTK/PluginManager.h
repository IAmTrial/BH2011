#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include "Plugin.h"


class PluginManager {
	private:
		std::list<Plugin*> plugins;
		std::wstring path;
		CRITICAL_SECTION crit;

	public:
		PluginManager(std::wstring pluginPath);
		~PluginManager();

		void Lock() { EnterCriticalSection(&crit); };
		void Unlock() { LeaveCriticalSection(&crit); };

		bool Load(std::wstring name);
		bool Unload(Plugin* plugin);

		bool DoesExist(std::wstring name);
		Plugin* GetPlugin(std::wstring name);

		__event void OnLoop();

		//__event void OnGameJoin(const string& name, const string& pass, int diff);
		__event void OnGameExit();

		__event void OnDraw();
		__event void OnAutomapDraw();
		__event void OnOOGDraw();

		__event void OnLeftClick(bool up, int x, int y, bool* block);
		__event void OnRightClick(bool up, int x, int y, bool* block);
		__event void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);

		__event void OnChatPacketRecv(BYTE* packet, bool* block);
		__event void OnRealmPacketRecv(BYTE* packet, bool* block);
		__event void OnGamePacketRecv(BYTE* packet, bool* block);
};