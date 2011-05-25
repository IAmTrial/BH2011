#include <Windows.h>
#include "D2Ptrs.h"

DWORD __declspec(naked) __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno)
{
	__asm 
	{
		mov eax, ecx;
		jmp D2CLIENT_GetUiVar_I;
	}
}
