#ifndef BASE_HPP
#define BASE_HPP


#include <string>

namespace RpgSaveLoadImpl {
    class Base;
}

class RpgSaveLoadImpl::Base
{
public:
	static const std::string SAVELOAD_CACHE_DB;
	static const std::string SAVELOAD_CACHE_TITLE;
	static const std::string SAVELOAD_CACHE_VENDOR;
	static const std::string SAVELOAD_CACHE_MYGAMES;
	static const std::string SAVELOAD_CACHE_VENDOR_PATH;
	static const std::string SAVELOAD_CACHE_TITLE_PATH;
	static const std::string SAVELOAD_CACHE_DIR_PATH;
	static const std::string SAVELOAD_CACHE_FILE_PATH;
};


#endif /* BASE_HPP */

