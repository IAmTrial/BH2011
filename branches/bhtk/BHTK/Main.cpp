#include "BHTK.h"
#include <Windows.h>

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, VOID* reserved) {
	BHTK* bhtk = BHTK::GetInstance();
	switch(reason) {
		case DLL_PROCESS_ATTACH:
			return bhtk->Startup(instance, reserved);
		break;
		case DLL_PROCESS_DETACH:
			return bhtk->Shutdown();
		break;
	}
}