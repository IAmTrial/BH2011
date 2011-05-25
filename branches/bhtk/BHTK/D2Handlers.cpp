#include "D2Ptrs.h"
#include "BHTK.h"
#include "D2Stubs.h"

Control* pD2WinEditBox;
void CheckD2WinEditBox();

void GameDraw() {
	__raise BHTK::pluginManager->OnDraw();
	//Drawing::UI::Draw();
	//Drawing::Hook::Draw(Drawing::InGame);
}

void GameAutomapDraw() {
	__raise BHTK::pluginManager->OnAutomapDraw();
}

void OOGDraw() {
	//Drawing::Hook::Draw(Drawing::OutOfGame);
	__raise BHTK::pluginManager->OnOOGDraw();
}

void GameLoop() {
	__raise BHTK::pluginManager->OnLoop();
}

DWORD WINAPI GameThread(VOID* lpvoid) {
	bool inGame = false;
	while(true) {
		if ((*p_D2WIN_FirstControl) && inGame) {
			inGame = false;
			__raise BHTK::pluginManager->OnGameExit();
		} else if (D2CLIENT_GetPlayerUnit() && !inGame) {
			inGame = true;
			//__raise BHTK::pluginManager->OnGameJoin((*p_D2LAUNCH_BnData)->szGameName, (*p_D2LAUNCH_BnData)->szGamePass, D2CLIENT_GetDifficulty());
		}
		Sleep(10);
	}
}

LONG WINAPI GameWindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	bool blockEvent = false;
	int mouseX = (*p_D2CLIENT_MouseX);
	int mouseY = (*p_D2CLIENT_MouseY);
	
	if (uMsg == WM_LBUTTONDOWN) {
		/*if (Drawing::Hook::LeftClick(false, mouseX, mouseY))
			blockEvent = true;
		if (Drawing::UI::LeftClick(false, mouseX, mouseY))
			blockEvent = true;*/
		__raise BHTK::pluginManager->OnLeftClick(false, mouseX, mouseY, &blockEvent);
	}

	if (uMsg == WM_LBUTTONUP) {
		/*if (Drawing::Hook::LeftClick(true, mouseX, mouseY))
			blockEvent = true;
		if (Drawing::UI::LeftClick(true, mouseX, mouseY))
			blockEvent = true;*/
		__raise BHTK::pluginManager->OnLeftClick(true, mouseX, mouseY, &blockEvent);
	}

	if (uMsg == WM_RBUTTONDOWN) {
		/*if (Drawing::Hook::RightClick(false, mouseX, mouseY))
			blockEvent = true;
		if (Drawing::UI::RightClick(false, mouseX, mouseY))
			blockEvent = true;*/
		__raise BHTK::pluginManager->OnRightClick(false, mouseX, mouseY, &blockEvent);
	}

	if (uMsg == WM_RBUTTONUP) {
		/*if (Drawing::Hook::RightClick(true, mouseX, mouseY))
			blockEvent = true;
		if (Drawing::UI::RightClick(true, mouseX, mouseY))
			blockEvent = true;*/
		__raise BHTK::pluginManager->OnRightClick(true, mouseX, mouseY, &blockEvent);
	}

	if (!D2CLIENT_GetUIState(0x05)) {
		if (uMsg == WM_KEYDOWN) {
		//	if (Drawing::Hook::KeyClick(false, wParam, lParam))
			//	return NULL;
			__raise BHTK::pluginManager->OnKey(false, wParam, lParam, &blockEvent);
		}

		if (uMsg == WM_KEYUP) {
			//if (Drawing::Hook::KeyClick(true, wParam, lParam))
			//	return NULL;
			__raise BHTK::pluginManager->OnKey(true, wParam, lParam, &blockEvent);
		}
	}

	return blockEvent ? NULL : (LONG)CallWindowProcA(BHTK::OldWNDPROC, hWnd, uMsg, wParam, lParam);
}

BOOL ChatPacketRecv(DWORD dwSize,BYTE* pPacket) {
	bool blockPacket = false;
	__raise BHTK::pluginManager->OnChatPacketRecv(pPacket, &blockPacket);
	return !blockPacket;
}

BOOL __fastcall RealmPacketRecv(BYTE* pPacket) {
	bool blockPacket = false;
	__raise BHTK::pluginManager->OnRealmPacketRecv(pPacket, &blockPacket);
	return !blockPacket;
}

DWORD __fastcall GamePacketRecv(BYTE* pPacket, DWORD dwSize) {
	if (!BHTK::cGuardLoaded && pPacket[0] == 0xAE) {
		return false;
	}
	bool blockPacket = false;
	__raise BHTK::pluginManager->OnGamePacketRecv(pPacket, &blockPacket);
	return !blockPacket;
}
