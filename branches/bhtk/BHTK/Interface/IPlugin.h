#pragma once
#include <string>

class IPlugin {
	public:
		virtual void OnLoad() {};
		virtual void OnUnload() {};
};

struct IPluginInfo {
	std::wstring name;
	std::wstring author;
	double version;
	IPlugin* plugin;
};