#include "D2Handlers.h"
#include "D2Ptrs.h"

VOID __declspec(naked) GameDraw_Interception()
{
	__asm
	{
		call GameDraw;

		POP ESI
		POP EBX
		POP ECX
		RETN 4
	}
}

void __declspec(naked) GameAutomapDraw_Interception()
{
   __asm
   {
      push eax;
      call GameAutomapDraw;
      pop eax;
      pop edi;
      pop esi;
      ret;
   }
} 

VOID __declspec(naked) OOGDraw_Interception()
{
	__asm
	{
		PUSHAD
		CALL OOGDraw
		POPAD
		MOV ESI,DWORD PTR DS:[ESI+0x3C]
		TEST ESI, ESI
		RETN
	}
}

void __declspec(naked) GameLoop_Interception()
{
	__asm
	{
		pushad;
		call GameLoop;
		popad;
 
		pop eax;
		sub esp, 0x20;
		mov [esp + 0xC], ecx;
		push eax;
		ret;
	}
}

VOID __declspec(naked) GamePacketRecv_Interception() {
	__asm
	{
		pop ebp;
		pushad;

		call GamePacketRecv;
		test eax, eax;

		popad;
		jnz OldCode;

		mov edx, 0;

OldCode:
		call D2NET_ReceivePacket_I;

		push ebp;
		ret;
	}
}

VOID __declspec(naked) ChatPacketRecv_Interception()
{
   __asm
   {
      pushad;
      call ChatPacketRecv;
      TEST EAX,EAX;

      popad;
      jnz oldCall;

        MOV EAX,0;
      MOV DWORD PTR DS:[EBX+0x6FF3ED58],1
      ret;
oldCall:
        CALL eax;
        MOV DWORD PTR DS:[EBX+0x6FF3ED58],1
        ret;

   }
}

VOID __declspec(naked) RealmPacketRecv_Interception()
{
	__asm
	{
		LEA ECX,DWORD PTR SS:[ESP+4]
		PUSHAD
		CALL RealmPacketRecv
		CMP EAX, 0
		POPAD
		JE Block
		CALL EAX
Block:
		RETN
	}
}