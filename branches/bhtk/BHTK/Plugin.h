#pragma once
#define EXPORTING
#include <string>
#include "Exports.h"

class Plugin {
	private:
		std::wstring name;
		std::wstring author;
		double version;
		bool loaded;
	public:
		EXPORT Plugin(std::wstring name, std::wstring author, double version);
		EXPORT ~Plugin();

		EXPORT std::wstring GetName() { return name; };
		EXPORT std::wstring GetAuthor() { return author; };
		EXPORT double GetVersion() { return version; };
		EXPORT bool IsLoaded() { return loaded; };

		EXPORT virtual void OnLoad() {};
		EXPORT virtual void OnUnload() {};
};
