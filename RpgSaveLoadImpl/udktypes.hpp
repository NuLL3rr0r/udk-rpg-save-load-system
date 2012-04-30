#ifndef UDKTYPES_HPP
#define UDKTYPES_HPP


#include <string>
#include <windows.h>

extern "C"
{
	typedef void *(*ReallocFunctionPtrType)(void *Original, DWORD Count, DWORD Alignment);
	extern ReallocFunctionPtrType ReallocFunctionPtr;

	struct FDLLBindInitData
	{
		INT Version;
		ReallocFunctionPtrType ReallocFunctionPtr;
	};

	__declspec(dllexport) void DLLBindInit(FDLLBindInitData *InitData);
}

struct FVector
{
	float x,y,z;
};

struct FString
{
	wchar_t *Data;
	int ArrayNum;
	int ArrayMax;

	void UpdateArrayNum();

	int Num();
	void Reallocate(int newNum, bool bCompact = false);
	FString *Clone() const;
	void Copy(const FString &other);
	const wchar_t *GetText() const;
	void SetText(const wchar_t *text);

	FString &operator = (const wchar_t *text);
	FString &operator = (const char *text);
	FString &operator = (const std::wstring &text);
	FString &operator = (const std::string &text);
};

template<typename DataType>
struct TArray
{
	DataType *Data;

	int Num()
	{
		return ArrayNum;
	}

	void Reallocate(int newNum, bool bCompact = false)
	{
		ArrayNum = newNum;
		if(ArrayNum > ArrayMax || bCompact) {
			ArrayMax = ArrayNum;
			Data = (DataType *)(*ReallocFunctionPtr)(Data, ArrayMax * sizeof(DataType), 8);
		}
	}

	void Clear()
	{
		Reallocate(0);
	}

	void AddItem(const DataType * const item)
	{
		Reallocate(ArrayNum + 1);
		Data[ArrayNum - 1] = *item;
	}

	const DataType &operator [] (unsigned index) const
	{
		assert(index >= 0 && index < ArrayMax);
		return Data[index];
	}

private:
	int ArrayNum;
	int ArrayMax;
};


#endif /* UDKTYPES_HPP */

