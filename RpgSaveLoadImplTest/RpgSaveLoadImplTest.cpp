// RpgSaveLoadImplTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "exportedfunctions.hpp"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	InitializeSaveLoad();
	RemoveSlot(L"QuickSave");
	return 0;
}

