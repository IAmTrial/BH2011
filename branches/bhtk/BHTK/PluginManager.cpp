#include "PluginManager.h"
#include "BHTK.h"
#include <io.h>
#include "Common.h"

using namespace std;

typedef Plugin* (__stdcall *loadPlugin)();

PluginManager::PluginManager(wstring pluginPath) : path(pluginPath) {
	InitializeCriticalSection(&crit);
}

PluginManager::~PluginManager() {
	DeleteCriticalSection(&crit);

	// Destroy all active plugins.
	while(plugins.size() > 0) {
		Unload(*plugins.begin());
	}
}

bool PluginManager::Load(wstring name) {
	// Make sure the plugin exists and isn't already loaded
	if (!DoesExist(name) || GetPlugin(name)) {
		return false;
	}

	Lock();

	wstring pluginPath = path + name + L".bhtk";

	// Load the plugin binary
	HMODULE module = LoadLibrary(pluginPath.c_str());
	if (!module) {
		Unlock();
		return false;
	}

	// Find the plugin initalizer
	loadPlugin initPlugin = (loadPlugin)GetProcAddress(module, "_InitPlugin@0");
	if (!initPlugin) {
		FreeLibrary(module);
		Unlock();
		return false;
	}

	// Initalize the plugin, returns information and plugin interface
	Plugin* plugin = initPlugin();
	if (!plugin) {
		FreeLibrary(module);
		Unlock();
		return false;
	}

	// Add the plugin to the list.
	plugins.push_back(plugin);

	plugin->Load();

	Unlock();

	return true;
}

bool PluginManager::Unload(Plugin* plugin) {
	return true;
}

bool PluginManager::DoesExist(wstring name) {
	wstring plugin = path + name + L".bhtk";
	char* path =  UnicodeToAnsi(plugin.c_str());
	bool exists = !(_access(path, 0) == -1);
	delete path;
	return exists;
}

Plugin* PluginManager::GetPlugin(wstring name) {
	for (list<Plugin*>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		if ((*it)->GetName().compare(name) == 0)
			return (*it);
	}
	return NULL;
}