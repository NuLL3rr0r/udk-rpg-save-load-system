#include <windows.h>
#include <cassert>
#include "util.hpp"
#include "udktypes.hpp"

#define MAX_STRING 1024

extern "C"
{
	ReallocFunctionPtrType ReallocFunctionPtr = NULL;

	__declspec(dllexport) void DLLBindInit(FDLLBindInitData *InitData)
	{
		ReallocFunctionPtr = InitData->ReallocFunctionPtr;
	}
}

void FString::UpdateArrayNum()
{
	ArrayNum = wcslen(Data) + 1;
	assert(ArrayNum <= ArrayMax);
}

int FString::Num()
{
	return ArrayNum;
}

void FString::Reallocate(int newNum, bool bCompact )
{
	ArrayNum = newNum;
	if( ArrayNum > ArrayMax || bCompact ) {
		ArrayMax = ArrayNum;
		Data = (wchar_t*)(*ReallocFunctionPtr)(Data, ArrayMax * sizeof(wchar_t), 8);
	}
}

FString *FString::Clone() const
{
	FString *pCopy = new FString();
	pCopy->Copy(*this);
	return pCopy;
}

void FString::Copy(const FString &other)
{
	SetText(other.GetText());
}

const wchar_t *FString::GetText() const
{
	if (ArrayMax == 0)
		return L"";

	wchar_t *result = new wchar_t[MAX_STRING];
	errno_t errorno = wcscpy_s(result, ArrayMax, Data);
	return result;
}

void FString::SetText(const wchar_t *text)
{
	size_t size = wcslen(text) + 1;
	int length = (int)size;
	if(length > ArrayMax) {
		Reallocate(length);
	}
	errno_t errorno = wcscpy_s(Data, ArrayMax, text);	
	UpdateArrayNum();
}

FString &FString::operator = (const wchar_t *text)
{
	SetText(text);
	return (*this);
}

FString& FString::operator = (const char *text)
{
	SetText(RpgSaveLoadImpl::Util::StrToWStr(text).c_str());
	return (*this);
}

FString &FString::operator = (const std::wstring &text)
{
	return (*this) = text.c_str();
}

FString &FString::operator = (const std::string &text)
{
	return (*this) = text.c_str();
}

