#include "ModuleManager.h"
#include "Module.h"

ModuleManager::ModuleManager() {

}

ModuleManager::~ModuleManager() {
	while(moduleList.size() > 0) {
		Module* module = moduleList.begin()->second;
		delete module;
	}
}

void ModuleManager::Add(Module* module) {
	// Add to list of modules
	moduleList[module->GetName()] = module;
}

void ModuleManager::Remove(Module* module) {
	// Remove module from list
	moduleList.erase(module->GetName());

	delete module;
}

void ModuleManager::LoadModules() {
	for (map<string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Load();
	}
}

void ModuleManager::UnloadModules() {
	for (map<string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Unload();
	}
}