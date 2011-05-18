#pragma once
#include <Windows.h>

void GameDraw();
void GameAutomapDraw();
void OOGDraw();

void GameLoop();
DWORD WINAPI GameThread(VOID* lpvoid);
LONG WINAPI GameWindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);