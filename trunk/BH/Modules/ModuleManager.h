#pragma once
#include <map>
#include <string>
#include <Windows.h>

class Module;
using namespace std;

class ModuleManager {
	private:
		map<string, Module*> moduleList;
	public:
		ModuleManager();
		~ModuleManager();

		// Module Management
		void Add(Module* module);
		void Remove(Module* module);

		void LoadModules();
		void UnloadModules();

		__event void OnLoop();

		__event void OnGameJoin(const string& name, const string& pass, int diff);
		__event void OnGameExit();

		__event void OnDraw();
		__event void OnAutomapDraw();
		__event void OnOOGDraw();

		__event void OnLeftClick(bool up, int x, int y, bool* block);
		__event void OnRightClick(bool up, int x, int y, bool* block);
		__event void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
};