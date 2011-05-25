#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <map>
#include "Common.h"
#define EXPORTING
#include "Exports.h"

using namespace std;

struct Toggle {
	unsigned int toggle;
	bool state;
};

class Config {
	private:
		std::wstring configName;
		std::map<std::wstring,std::wstring> contents;

	public:
		EXPORT Config(std::wstring name) : configName(name) {};
		EXPORT Config(map<std::wstring, std::wstring> content) : contents(content) {};

		//Parse the config file and store results
		EXPORT bool Parse();
		EXPORT bool Write();

		//Functions to read values from the configuration
		EXPORT bool				ReadBoolean	(std::wstring key, bool value);
		EXPORT std::wstring		ReadString	(std::wstring key, std::wstring value) { return (contents[key].size() == 0) ? value : contents[key]; };
		EXPORT int					ReadInt		(std::wstring key, int value);
		EXPORT unsigned int		ReadKey		(std::wstring key, std::wstring toggle);
		EXPORT Toggle				ReadToggle	(std::wstring key, std::wstring toggle, bool defaultState);
		EXPORT std::vector<wstring> ReadArray	(std::wstring key);
		EXPORT map<wstring, wstring> ReadAssoc	(std::wstring key);

		//Functions to write values to the configuration
		EXPORT void				WriteBoolean(std::wstring key, bool value) {};

};