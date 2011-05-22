#include <Windows.h>
#include <iostream>
#include "../BHTK/Interface/IPlugin.h"
#include "../BHTK/Interface/IBHTK.h"
using namespace std;

class PluginTest : public IPlugin {
public:
	PluginTest() {};

	void OnLoad();
};

IBHTK* BHTK;


void PluginTest::OnLoad() {
	BHTK->Print("Hello World!");
}

BOOL WINAPI DllMain(HINSTANCE hDll,DWORD dwReason,LPVOID lpReserved) {
	return true;
}

extern "C" __declspec(dllexport) IPluginInfo* __stdcall InitPlugin(IBHTK* bhtk) 
{
	BHTK = bhtk;
	IPluginInfo* info = new IPluginInfo;
	info->name = "Test Plugin";
	info->author = "McGod";
	info->plugin = new PluginTest();
	info->version = 1.0;
	return info;
}