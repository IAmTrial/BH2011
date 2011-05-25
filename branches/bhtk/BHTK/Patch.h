#pragma once

#define EXPORTING

#include <vector>
#include <string>
#include <Windows.h>
#include "Exports.h"

class Patch;


enum Dll { D2CLIENT=0,D2COMMON,D2GFX,D2LANG,D2WIN,D2NET,D2GAME,D2LAUNCH,FOG,BNCLIENT, STORM, D2CMP, D2MULTI, D2MCPCLIENT};
enum PatchType { Jump=0, Call };


class Patch {
	private:
		static std::vector<Patch*> Patchs;
		Dll dll;
		PatchType type;
		int offset, length, function;
		BYTE* oldCode;
		bool injected;
	public:
		EXPORT Patch(PatchType type, Dll dll, int offset, int function, int length);

		EXPORT bool Install ();
		EXPORT bool Remove ();

		EXPORT bool IsInstalled() { return injected; };

		EXPORT static int GetDllOffset(Dll dll, int offset);
		static bool WriteBytes(int address, int len, BYTE* bytes);
};