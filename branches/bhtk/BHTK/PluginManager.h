#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include "Plugin.h"


class PluginManager {
	private:
		std::list<Plugin*> plugins;
		std::string path;
		CRITICAL_SECTION crit;

	public:
		PluginManager(std::string pluginPath);
		~PluginManager();

		void Lock() { EnterCriticalSection(&crit); };
		void Unlock() { LeaveCriticalSection(&crit); };

		bool Load(std::string name);
		bool Unload(Plugin* plugin);

		bool DoesExist(std::string name);
		Plugin* GetPlugin(std::string name);
};