#ifndef UTIL_HPP
#define UTIL_HPP


#include <string>
#include <windows.h>

namespace RpgSaveLoadImpl {
    class Util;
}

class RpgSaveLoadImpl::Util
{
public:
	static std::wstring StrToWStr(const std::string &str);
	static std::string WStrToStr(const std::wstring &wstr);
};


#endif /* UTIL_HPP */

