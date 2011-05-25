#pragma once
#define EXPORTING

#include "PluginManager.h"
#include <Windows.h>
#include "Exports.h"
#include "Drawing.h"
#include "Config.h"

struct cGuardModule
{	
	union {
		HMODULE hModule;
		DWORD dwBaseAddress;
	};
	DWORD _1;
	char szPath[MAX_PATH];
};

namespace BHTK {
	VAR std::wstring path;
	VAR HINSTANCE instance;
	VAR bool cGuardLoaded;
	VAR PluginManager* pluginManager;
	VAR WNDPROC OldWNDPROC;
	VAR Drawing::UI* settingsUI;
	VAR Config* config;

	bool Startup(HMODULE instance, VOID* reserved);
	bool Shutdown();

	// Interface functions
	EXPORT void Print(std::wstring text);
	EXPORT std::wstring GetPath();
	EXPORT bool IsGuarded();
	EXPORT Drawing::UI* GetSettingsUI();
	EXPORT Config* GetConfig();
};
