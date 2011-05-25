#pragma once
#define EXPORTING

#include "PluginManager.h"
#include <Windows.h>
#include "Exports.h"

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
	EXPORT VAR std::wstring path;
	VAR HINSTANCE instance;
	EXPORT VAR bool cGuardLoaded;
	VAR PluginManager* pluginManager;
	VAR WNDPROC OldWNDPROC;

	bool Startup(HMODULE instance, VOID* reserved);
	bool Shutdown();

	// Interface functions
	EXPORT void Print(std::wstring text);
};
