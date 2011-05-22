#include <iostream>
#include <string>
#include <Shlwapi.h>
#define _DEFINE_PTRS
#include "D2Ptrs.h"
#include "Patch.h"
#include "BHTK.h"

BHTK* BHTK::bhtk = NULL;

bool BHTK::Startup(HMODULE instance, VOID* reserved) {
	BHTK::instance = instance;
	if (reserved != NULL) {
		/*cGuardModule* pModule = (cGuardModule*)reserved;
		if(!pModule)
			return FALSE;
		path.assign(pModule->szPath);
		cGuardLoaded = true;*/
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

BHTK* BHTK::GetInstance() {
	if (!bhtk) 
		bhtk = new BHTK();
	return bhtk;
}

IPatch* BHTK::NewPatch(PatchType type, Dll dll, int offset, int function, int length) {
	return new Patch(type, dll, offset, function, length);
}