#define _DEFINE_VAR
#include "BHTK.h"
#include <Windows.h>

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, VOID* reserved) {
	switch(reason) {
		case DLL_PROCESS_ATTACH:
			return BHTK::Startup(instance, reserved);
		break;
		case DLL_PROCESS_DETACH:
			return BHTK::Shutdown();
		break;
	}
}