#ifndef SYSTEM_HPP
#define SYSTEM_HPP


#include <string>

namespace RpgSaveLoadImpl {
    class System;
}

class RpgSaveLoadImpl::System
{
public:
	static std::string DirectorySeparatorChar();
	static std::string GetCurrentPath();
	static std::string GetFolderPath(int specialFolder);
};


#endif /* SYSTEM_HPP */

