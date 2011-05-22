#pragma once
#include <string>

class IPlugin {
	public:
		virtual void OnLoad() {};
		virtual void OnUnload() {};
};

struct IPluginInfo {
	std::string name;
	std::string author;
	double version;
	IPlugin* plugin;
};