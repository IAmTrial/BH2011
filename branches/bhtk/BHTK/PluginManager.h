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
};