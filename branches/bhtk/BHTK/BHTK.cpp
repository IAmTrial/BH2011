#define _DEFINE_PTRS
#include <iostream>
#include <string>
#include <Shlwapi.h>
#include "D2Ptrs.h"
#include "Patch.h"
#include "BHTK.h"
#include "Common.h"

bool BHTK::Startup(HMODULE instance, VOID* reserved) {
	BHTK::instance = instance;
	if (reserved != NULL) {
		cGuardModule* pModule = (cGuardModule*)reserved;
		if(!pModule)
			return FALSE;
		wchar_t* wPath = AnsiToUnicode(pModule->szPath);
		path.assign(wPath);
		delete wPath;
		cGuardLoaded = true;
	} else {
		wchar_t szPath[MAX_PATH];
		GetModuleFileName(BHTK::instance, szPath, MAX_PATH);
		PathRemoveFileSpec(szPath);
		path.assign(szPath);
		path += L"\\";
		cGuardLoaded = false;
	}

	pluginManager = new PluginManager(path);

	pluginManager->Load(L"TestPlugin");

	return true;
}

bool BHTK::Shutdown() {
	return true;
}

void BHTK::Print(std::wstring text) {
	D2CLIENT_PrintGameString((wchar_t*)text.c_str(), 0);
}