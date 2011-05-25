#include "Plugin.h"

Plugin::Plugin(std::wstring name, std::wstring author, double version) : name(name), author(author), version(version) {
	loaded = false;
}

Plugin::~Plugin() {
}