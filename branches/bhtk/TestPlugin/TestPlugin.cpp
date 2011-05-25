#include <Windows.h>
#include <iostream>
#define _DEFINE_PTRS
#include "BHTK.h"
#include "Patch.h"
#include "D2Ptrs.h"

using namespace std;

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

class PluginTest : public Plugin {
	private:
		Patch* infravision;
	public:
		PluginTest();

		void OnLoad();

		void OnOOGDraw();
};

PluginTest::PluginTest() : Plugin(L"Test Plugin", L"McGod", 1.0) {
	infravision = new Patch(Call, D2CLIENT, 0x66623, (int)Infravision_Interception, 7);
}

void PluginTest::OnLoad() {
	infravision->Install();
	BHTK::Print(L"PluginTest: Patched Infravision");
}

void PluginTest::OnOOGDraw() {
	D2WIN_DrawText(L"Test", 100, 100, 3, 0);
}

BOOL WINAPI DllMain(HINSTANCE hDll,DWORD dwReason,LPVOID lpReserved) {
	return true;
}

extern "C" __declspec(dllexport) Plugin* __stdcall InitPlugin() 
{
	return new PluginTest;
}