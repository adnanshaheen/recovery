//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : APIs
//

#ifndef MG_API_HEADER
#define MG_API_HEADER

//
// APIs
//

#include "CoreString.h"

/**
 * Get/Set last error number of application
 */
static int g_err_no = 0;
static int GetErrorNumber()
{
	return g_err_no;
}
static void SetErrorNumber(int err_no)
{
	g_err_no = err_no;
}

// Get error message from error code.
static CCoreString GetErrorMessage(DWORD dwErrorCode)
{
#ifdef WIN32
	LPVOID lpMsgBuf = nullptr;

	// We will use default language so the user can also read it.
	DWORD dwResult = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									nullptr,
									dwErrorCode,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPTSTR) &lpMsgBuf,
									0,
									nullptr);

	// If the FormatMessage has been failed.
	if (dwResult == 0 || !lpMsgBuf) {
		// Free the message string
		if (lpMsgBuf) {
			LocalFree(lpMsgBuf);
			lpMsgBuf = nullptr;
		}
		return _T("");
	}

	CCoreString csErrMsg((LPCTSTR) lpMsgBuf);
	LocalFree(lpMsgBuf);
	lpMsgBuf = nullptr;

	// Remove any new line or extra space characters.
	csErrMsg.TrimRight();
	return csErrMsg;
#else
	return CCoreString(strerror(dwErrorCode));
#endif // WIN32
}

// Get unique file name.
static CCoreString GetUniqueFileName(LPCTSTR pszFilePath = nullptr, LPCTSTR pszExtension = nullptr)
{
	DWORD dwSize = GetShortPathName(pszFilePath, nullptr, 0);
	std::vector<TCHAR> szShortFilePath(dwSize + 1);
	if (GetShortPathName(pszFilePath, &szShortFilePath[0], dwSize)) {
		dwSize = MAX_PATH * 2;
		std::vector<TCHAR> filePath(dwSize);
		if (GetTempFileName(&szShortFilePath[0], _T("MGRY"), 0, &filePath[0])) {
			CCoreString csFileName = &filePath[0];

			// Delete the created file, we only want the unique file name
			::DeleteFile(csFileName);

			if (pszExtension) {
				int iPos = csFileName.ReverseFind(_T('.'));
				csFileName = csFileName.Left(iPos + 1);
				csFileName += pszExtension;
			}
			return csFileName;
		}
	}
	return _T("");
}

static CCoreString fnGetModuleFileName(HMODULE hModule)
{
#ifndef WINPE
	std::vector<TCHAR> filePath(MAX_PATH * 2);
	::GetModuleFileName(hModule, &filePath[0], MAX_PATH * 2);
	return (&filePath[0]);
#else
	CCoreString csPath;
	csPath = kszPath;
	csPath += kszApp;
	return csPath;
#endif
}

//
// End of APIs declaration
//

#endif // MG_API_HEADER

//
// End of header.
//
