#include <shlobj.h>
#include <windows.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/format.hpp>
#include "system.hpp"

using namespace std;
using namespace boost;
using namespace RpgSaveLoadImpl;

string System::DirectorySeparatorChar()
{
	static string separator = "\\";
	return separator;
}

string System::GetCurrentPath()
{
	char pathBuffer[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
	string path(pathBuffer);

	if (result != ERROR_SUCCESS) {
		string::size_type pos = path.find_last_of(DirectorySeparatorChar());
		path = path.substr(0, pos);
	} else {
		MessageBoxA(0, (format("Error code: %1") % result).str().c_str(), "IO Error", MB_OK);
	}

    return path;
}

string System::GetFolderPath(int specialFolder)
{
	char pathBuffer[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, specialFolder, NULL, SHGFP_TYPE_CURRENT, pathBuffer);
	string path(pathBuffer);

    if (result == S_OK) {
		if (algorithm::ends_with(path, DirectorySeparatorChar())) {
			path = path.substr(0, path.length() - 2);
		}
	} else {
		MessageBoxA(0, (format("Error code: %1") % result).str().c_str(), "IO Error", MB_OK);
	}

	return path;
}

