#include <iostream>
#include <Shlwapi.h>
#include "PluginManager.h"

int main(int argc, char** argv) {
	PathRemoveFileSpecA(argv[0]);
	std::string path = argv[0];
	path += "\\";
	PluginManager* manager = new PluginManager(path);
	
	manager->Load("TestPlugin");

	system("PAUSE");
}