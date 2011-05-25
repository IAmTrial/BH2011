#pragma once
#include <string>
#include <vector>
#include <sstream>

void Tokenize(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters);
wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
template <class T>
bool from_string(T& t, 
                 const std::wstring& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::wistringstream iss(s);
  return !(iss >> f >> t).fail();
}

template< class type> std::wstring to_string( const type & value)
{ std::wstringstream ss; ss << value; return ss.str(); }

bool StringToBool(std::wstring str);

std::wstring Trim(std::wstring source);

struct KeyCode {
	std::wstring name;
	unsigned int value;
	std::wstring literalName;
};

KeyCode GetKeyCode(unsigned int nKey);
KeyCode GetKeyCode(const wchar_t* name);