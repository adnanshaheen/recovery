//
// Project : LaserRecovery
//
// Author : Adnan Shaheen
//
//
// Purpose : Do not submit any code before its been tested from here.
//

#include "Types.h"
#include "CoreString.h"
#include "DateTime.h"

#include <iostream>

int main(char* argv[], int argc)
{
	CDateTime cDateTime;
	cDateTime.GetSystemTime();

	WORD wYear = 0;
	WORD wMonth = 0;
	WORD wDay = 0;
	cDateTime.GetDate(wYear, wMonth, wDay);
	CCoreString csCurrentDateTime;
	csCurrentDateTime.Format(_T("System Current Date is %d-%d-%d\n"), wYear, wMonth, wDay);

	WORD wHour = 0;
	WORD wMinute = 0;
	WORD wSecond = 0;
	cDateTime.GetTime(wHour, wMinute, wSecond);
	csCurrentDateTime.Format(_T("System Current Time is %d:%d:%d\n"), wHour, wMinute, wSecond);

	CCoreString csLibrary;
#ifdef DEBUG
	csLibrary = _T("../../lib/Debug/DiskBoard.dll");
#else
	csLibrary = _T("../../lib/Release/DiskBoard.dll");
#endif // DEBUG
	HMODULE hModule = LoadLibrary(csLibrary);
	if (hModule == NULL || hModule == INVALID_HANDLE_VALUE)
	{
		// Library could not be loaded
		DWORD dwError = GetLastError();
		CCoreString csError;
		csError.Format(_T("Error %d occured while opening library DiskBoard.dll"), dwError);
		ASSERT(FALSE);
		return -1;
	}

	FreeLibrary(hModule);

	return 0;
}

// End of test.
//
