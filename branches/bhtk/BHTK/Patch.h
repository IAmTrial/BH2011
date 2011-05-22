#pragma once

#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class Patch;

#include "Interface\IPatch.h"

class Patch : public IPatch {
	private:
		static std::vector<Patch*> Patchs;
		Dll dll;
		PatchType type;
		int offset, length, function;
		BYTE* oldCode;
		bool injected;
	public:
		Patch(PatchType type, Dll dll, int offset, int function, int length);

		bool Install ();
		bool Remove ();

		bool IsInstalled() { return injected; };

		static int GetDllOffset(Dll dll, int offset);
		static bool WriteBytes(int address, int len, BYTE* bytes);
};