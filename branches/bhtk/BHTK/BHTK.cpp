#include <iostream>
#include <string>
#include "BHTK.h"

BHTK* BHTK::bhtk = NULL;

void BHTK::Print(std::string text) {
	std::cout << text << std::endl;
}

BHTK* BHTK::GetInstance() {
	if (!bhtk) 
		bhtk = new BHTK();
	return bhtk;
}