#pragma once
#include <string>
#include "Interface/IPlugin.h"

class Plugin {
	private:
		IPlugin* pluginInterface;
		std::string name;
		std::string author;
		double version;
		bool loaded;
	public:
		Plugin(IPluginInfo* info);
		~Plugin();

		std::string GetName() { return name; };
		std::string GetAuthor() { return author; };
		double GetVersion() { return version; };
		bool IsLoaded() { return loaded; };

};
