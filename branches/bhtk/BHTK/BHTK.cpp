#define _DEFINE_PTRS
#include <iostream>
#include <string>
#include <Shlwapi.h>
#include "D2Ptrs.h"
#include "Patch.h"
#include "BHTK.h"
#include "Common.h"
#include "D2Handlers.h"
#include "D2Intercepts.h"
#include "Drawing.h"

Patch* patches[] = {
	new Patch(Call, D2CLIENT, 0x44230, (int)GameLoop_Interception, 7),

	new Patch(Jump, D2CLIENT, 0xC3DB4,	(int)GameDraw_Interception, 6),
	new Patch(Jump, D2CLIENT, 0x626C9, (int)GameAutomapDraw_Interception, 5),
	new Patch(Call, D2WIN, 0x18911, (int)OOGDraw_Interception, 5),

	new Patch(Call, BNCLIENT, 0xEABC, (int)ChatPacketRecv_Interception, 12),
	new Patch(Call, D2MCPCLIENT, 0x69D7, (int)RealmPacketRecv_Interception, 5),
	new Patch(Call, D2CLIENT, 0xACE61, (int)GamePacketRecv_Interception, 5),
};

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
	config = new Config(L"BHTK.cfg");
	config->Parse();

	for (int n = 0; n < (sizeof(patches) / sizeof(Patch*)); n++) {
		patches[n]->Install();
	}

	if(D2GFX_GetHwnd()) {
		BHTK::OldWNDPROC = (WNDPROC)GetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC);
		SetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC,(LONG)GameWindowEvent);
	}

	CreateThread(0,0,GameThread, 0,0,0);

	BHTK::settingsUI = new Drawing::UI(L"Settings", 350, 200);

	pluginManager->Load(L"TestPlugin");

	return true;
}

bool BHTK::Shutdown() {
	return true;
}

void BHTK::Print(std::wstring text) {
	D2CLIENT_PrintGameString((wchar_t*)text.c_str(), 0);
}

std::wstring BHTK::GetPath() {
	return path;
}

bool BHTK::IsGuarded() {
	return cGuardLoaded;
}

Drawing::UI* BHTK::GetSettingsUI() {
	return settingsUI;
}

Config* BHTK::GetConfig() {
	return config;
}