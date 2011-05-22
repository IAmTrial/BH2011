#include "Plugin.h"

Plugin::Plugin(IPluginInfo* info) {
	name = info->name;
	author = info->author;
	version = info->version;
	pluginInterface = info->plugin;
	loaded = false;

	pluginInterface->OnLoad();
}

Plugin::~Plugin() {
	delete pluginInterface;
}