#ifndef EXPORTEDFUNCTIONS_HPP
#define EXPORTEDFUNCTIONS_HPP


#include "slotdata.hpp"

extern "C"
{
	__declspec(dllexport) void InitializeSaveLoad();
	__declspec(dllexport) void GetAllSlots(struct SlotListStruct *out_slotsList);
	__declspec(dllexport) void CommitSlot(const wchar_t *id, struct SlotDataStruct *data);
	__declspec(dllexport) void FetchSlot(const wchar_t *id, struct SlotDataStruct *out_data);
	__declspec(dllexport) void RemoveSlot(const wchar_t *id);
}


#endif /* EXPORTEDFUNCTIONS_HPP */


