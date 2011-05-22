#include "PluginManager.h"
#include "BHTK.h"
#include <io.h>

using namespace std;

typedef IPluginInfo* (__stdcall *loadPlugin)(BHTK*);

PluginManager::PluginManager(string pluginPath) : path(pluginPath) {
	InitializeCriticalSection(&crit);
}

PluginManager::~PluginManager() {
	DeleteCriticalSection(&crit);

	// Destroy all active plugins.
	while(plugins.size() > 0) {
		Unload(*plugins.begin());
	}
}

bool PluginManager::Load(string name) {
	if (!DoesExist(name) || GetPlugin(name)) {
		return false;
	}

	Lock();

	string pluginPath = path + name + ".bhtk";

	HMODULE module = LoadLibrary(pluginPath.c_str());
	if (!module) {
		Unlock();
		return false;
	}

	loadPlugin initPlugin = (loadPlugin)GetProcAddress(module, "_InitPlugin@4");
	if (!initPlugin) {
		Unlock();
		return false;
	}

	IPluginInfo* info = initPlugin(BHTK::GetInstance());
	if (!info) {
		Unlock();
		return false;
	}

	plugins.push_back(new Plugin(info));
	delete info;

	Unlock();

	return true;
}

bool PluginManager::Unload(Plugin* plugin) {
	return true;
}

bool PluginManager::DoesExist(string name) {
	string plugin = path + name + ".bhtk";
	return !(_access(plugin.c_str(), 0) == -1);
}

Plugin* PluginManager::GetPlugin(string name) {
	for (list<Plugin*>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		if ((*it)->GetName().compare(name) == 0)
			return (*it);
	}
	return NULL;
}