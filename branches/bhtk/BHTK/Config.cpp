#include "Config.h"
#include "BHTK.h"
#include <sstream>

/* Parse()
	Parse the configuration file and stores the results in a key->value pair.
	Can be called multiple times so you can reload the configuration.
*/
bool Config::Parse() {
	
	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	wfstream file(BHTK::path + configName);
	if (!file.is_open())
		return false;

	//Lock Critical Section then wipe contents incase we are reloading.
	contents.erase(contents.begin(), contents.end());

	//Begin to loop the configuration file one line at a time.
	wstring line;
	wchar_t szLine[2048];
	while(!file.eof()) {
		//Get the current line from the file.
		file.getline(szLine, 2048);
		line = szLine;

		//Remove any comments from the config
		if (line.find_first_of(L"//") != wstring::npos)
			line = line.erase(line.find_first_of(L"//"));

		//Insure we have something in the line now.
		if (line.length() == 0)
			continue;

		//Grab the Key and Value
		wstring key = Trim(line.substr(0, line.find_first_of(L":")));
		wstring value = Trim(line.substr(line.find_first_of(L":") + 1));

		//Store them!
		contents.insert(pair<wstring, wstring>(key,value));
	}
	return true;
}

bool Config::Write() {
	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	wfstream file(BHTK::path + configName);
	if (!file.is_open())
		return false;

	//Read in the configuration value
	vector<wstring> configLines;
	wchar_t line[2048];
	while(!file.eof()) {
		file.getline(line, 2048);
		configLines.push_back(line);
	}

	for (vector<wstring>::iterator it = configLines.begin(); it < configLines.end(); it++) {
		//Remove any comments from the config
		if ((*it).find_first_of(L"//") != wstring::npos)
			(*it) = (*it).erase((*it).find_first_of(L"//"));

		//Insure we have something in the line now.
		if ((*it).length() == 0)
			continue;
	}

	return true;
}

/* ReadBoolean(std::wstring key, bool value)
 *	Reads in a boolean from the key-pair.
 */
bool Config::ReadBoolean(std::wstring key, bool value) {
	//Check if configuration value exists
	if (contents[key].length() == 0)
		return value;
	//Convert wstring to boolean
	const wchar_t* szValue = contents[key].c_str();
	if ((_wcsicmp(szValue, L"1") == 0) || (_wcsicmp(szValue, L"y") == 0) || (_wcsicmp(szValue, L"yes") == 0) || (_wcsicmp(szValue, L"true") == 0))
		return true;
	return false;
}

/* ReadInt(std::wstring key, int value)
 *	Reads in a decimal or hex(which is converted to decimal) from the key-pair.
 */
int Config::ReadInt(std::wstring key, int value) {
	//Check if configuration value exists
	if (contents[key].length() == 0)
		return value;
	//Use a wstring stream to convert values
	wstringstream convert;
	int ret;
	//Hack todo hex conversion
	if (!contents[key].find(L"0x"))
		from_string<int>(ret,contents[key],std::hex);
	else
		from_string<int>(ret,contents[key],std::dec);
	return ret;
}

/* ReadToggle(std::wstring key, std::wstring toggle, bool state)
 *	Reads in a toggle from the key->pair
 *	Config Example:
 *		Key: True, VK_A
 */
Toggle Config::ReadToggle(std::wstring key, std::wstring toggle, bool state) {
	//Check if configuration value exists.
	Toggle ret;
	ret.toggle = GetKeyCode(toggle.c_str()).value;
	ret.state = state;
	if (contents[key].length() == 0)
		return ret;

	//Pull the keycode from after the comma.
	ret.toggle = GetKeyCode(Trim(contents[key].substr(contents[key].find_first_of(L",") + 1)).c_str()).value;
	
	//Pull the state from before the comma
	wstring boolean = Trim(contents[key].substr(0, contents[key].find_first_of(L",")));
	const wchar_t* newState = boolean.c_str();
	
	//Convert state to boolean
	if ((_wcsicmp(newState, L"1") == 0) || (_wcsicmp(newState, L"y") == 0) || (_wcsicmp(newState, L"yes") == 0) || (_wcsicmp(newState, L"true") == 0))
		ret.state = true;
	else
		ret.state = false;

	return ret;
}

/* ReadKey(std::wstring key, std::wstring toggle)
 *	Reads in a key from the key->pair.
 */
unsigned int Config::ReadKey(std::wstring key, std::wstring toggle) {
	//Check if configuration value exists.
	if (contents[key].length() == 0)
		return GetKeyCode(toggle.c_str()).value;

	//Grab the proper key code and make s ure it's valid
	KeyCode ret = GetKeyCode(contents[key].c_str());
	if (ret.value == 0)
		return GetKeyCode(toggle.c_str()).value;

	return ret.value;
}

/* ReadArray(std::wstring key)
 *	Reads in a index-based array from the array
 */
vector<wstring> Config::ReadArray(std::wstring key) {
	vector<wstring> ret;
	int currentIndex = 0;
	while(true) {
		wstringstream index;
		index << currentIndex;
		wstring currentKey = key + L"[" + index.str() + L"]";
		if (contents[currentKey].length() == 0)
			return ret;
		ret.push_back(contents[currentKey]);
		currentIndex++;
	}
	//Never should occur
	return ret;
}

/* ReadGroup(std::wstring key)
 *	Reads in a map from the key->pair
 *	Config Example:
 *		Value[Test]: 0
 *		Value[Pickles]: 1
 */
map<wstring, wstring> Config::ReadAssoc(std::wstring key) {
	map<wstring, wstring> ret;

	for (map<wstring, wstring>::iterator it = contents.begin(); it != contents.end(); it++) {
		if (!(*it).first.find(key + L"[")) {
			pair<wstring,wstring> assoc;
			//Pull the value from between the []'s
			assoc.first = (*it).first.substr((*it).first.find(L"["), (*it).first.length() - (*it).first.find(L"[") - 1);
			//Simply store the value that was after the :
			assoc.second = (*it).second;
			ret.insert(assoc);
		}
	}

	return ret;
}