#include <Windows.h>
#include <iostream>
#include "../BHTK/Interface/IBHTK.h"

using namespace std;

IBHTK* BHTK;
struct UnitAny;

BOOL __fastcall InfravisionPatch(UnitAny *unit)
{
	return false;
}

void __declspec(naked) Infravision_Interception()
{
	__asm {
		mov ecx, esi
		call InfravisionPatch
		add dword ptr [esp], 0x72
		ret
	}
}

class PluginTest : public IPlugin {
	private:
		IPatch* infravision;
	public:
		PluginTest();

		void OnLoad();
};

PluginTest::PluginTest() {
	infravision = BHTK->NewPatch(Call, D2CLIENT, 0x66623, (int)Infravision_Interception, 7);
}

void PluginTest::OnLoad() {
	infravision->Install();
	BHTK->Print(L"Infravision Patch installed");
}

BOOL WINAPI DllMain(HINSTANCE hDll,DWORD dwReason,LPVOID lpReserved) {
	return true;
}

extern "C" __declspec(dllexport) IPluginInfo* __stdcall InitPlugin(IBHTK* bhtk) 
{
	BHTK = bhtk;
	IPluginInfo* info = new IPluginInfo;
	info->name = L"Test Plugin";
	info->author = L"McGod";
	info->plugin = new PluginTest();
	info->version = 1.0;
	return info;
}