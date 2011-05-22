#pragma once
#include "Interface\IBHTK.h"
#include "Interface\IPatch.h"
#include "PluginManager.h"
#include <Windows.h>

struct cGuardModule
{	
	union {
		HMODULE hModule;
		DWORD dwBaseAddress;
	};
	DWORD _1;
	char szPath[MAX_PATH];
};

class BHTK : public IBHTK {
	private:
		static BHTK* bhtk;
		std::wstring path;
		HINSTANCE instance;
		bool cGuardLoaded;
		PluginManager* pluginManager;
	public:
		bool Startup(HMODULE instance, VOID* reserved);
		bool Shutdown();

		// Interface functions
		void Print(std::wstring text);

		// Class Factories
		IPatch* NewPatch(PatchType type, Dll dll, int offset, int function, int length);

		static BHTK* GetInstance();
};
