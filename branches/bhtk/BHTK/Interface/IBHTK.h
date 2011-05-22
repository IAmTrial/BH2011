#pragma once
#include <string>
#include "IPatch.h"
#include "IPlugin.h"

class IBHTK {
	public:

	virtual void Print(std::wstring text) = 0;

	virtual IPatch* NewPatch(PatchType type, Dll dll, int offset, int function, int length) = 0;
};