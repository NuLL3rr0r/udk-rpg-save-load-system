#include <string>
#include "base.hpp"
#include "system.hpp"

using namespace std;
using namespace RpgSaveLoadImpl;

const string Base::SAVELOAD_CACHE_DB = "cache.db";
const string Base::SAVELOAD_CACHE_TITLE = "Thrateon";
const string Base::SAVELOAD_CACHE_VENDOR = "RasGames";
const string Base::SAVELOAD_CACHE_MYGAMES = "My Games";
const string Base::SAVELOAD_CACHE_VENDOR_PATH = Base::SAVELOAD_CACHE_MYGAMES + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_VENDOR;
const string Base::SAVELOAD_CACHE_TITLE_PATH = Base::SAVELOAD_CACHE_MYGAMES + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_VENDOR + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_TITLE;
const string Base::SAVELOAD_CACHE_DIR_PATH = Base::SAVELOAD_CACHE_MYGAMES + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_VENDOR + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_TITLE;
const string Base::SAVELOAD_CACHE_FILE_PATH = Base::SAVELOAD_CACHE_MYGAMES + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_VENDOR + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_TITLE + System::DirectorySeparatorChar()
												+ Base::SAVELOAD_CACHE_DB;

