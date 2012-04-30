#include <windows.h>
#include "debug.hpp"
#include "exportedfunctions.hpp"
#include "saveload.hpp"
#include "util.hpp"

using namespace RpgSaveLoadImpl;

SaveLoad saveLoadObject;
bool isInitialized = false;

extern "C"
{
	__declspec(dllexport) void InitializeSaveLoad()
	{
		if (!isInitialized) {
			saveLoadObject.Initialize();
			isInitialized = true;
		}
	}

	__declspec(dllexport) void GetAllSlots(struct SlotListStruct *out_slotsList)
	{
		saveLoadObject.GetAllSlots(out_slotsList);
	}

	__declspec(dllexport) void CommitSlot(const wchar_t *id, struct SlotDataStruct *data)
	{
		saveLoadObject.CommitSlot(Util::WStrToStr(id), data);
	}

	__declspec(dllexport) void FetchSlot(const wchar_t *id, struct SlotDataStruct *out_data)
	{
		saveLoadObject.FetchSlot(Util::WStrToStr(id), out_data);
	}

	__declspec(dllexport) void RemoveSlot(const wchar_t *id)
	{
		saveLoadObject.RemoveSlot(Util::WStrToStr(id));
	}
}

