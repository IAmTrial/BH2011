#pragma once
#include <string>
#include "Interface/IPlugin.h"

class Plugin {
	private:
		IPlugin* pluginInterface;
		std::wstring name;
		std::wstring author;
		double version;
		bool loaded;
	public:
		Plugin(IPluginInfo* info);
		~Plugin();

		std::wstring GetName() { return name; };
		std::wstring GetAuthor() { return author; };
		double GetVersion() { return version; };
		bool IsLoaded() { return loaded; };

};
