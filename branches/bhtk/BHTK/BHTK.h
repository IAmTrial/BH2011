#pragma once
#include "Interface\IBHTK.h"

class BHTK : public IBHTK {
	public:

		void Print(std::string text);

		static BHTK* GetInstance();
	private:
		static BHTK* bhtk;
};
