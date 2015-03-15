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

// Get error message from error code.
static CCoreString GetErrorMessage(DWORD dwErrorCode)
{
#ifdef WIN32
	LPVOID lpMsgBuf = NULL;

	// We will use default language so the user can also read it.
	DWORD dwResult = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									NULL,
									dwErrorCode,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPTSTR) &lpMsgBuf,
									0,
									NULL);

	// If the FormatMessage has been failed.
	if (dwResult == 0 || !lpMsgBuf) {
		// Free the message string
		if (lpMsgBuf) {
			LocalFree(lpMsgBuf);
			lpMsgBuf = NULL;
		}
		return _T("");
	}

	CCoreString csErrMsg((LPCTSTR) lpMsgBuf);
	LocalFree(lpMsgBuf);
	lpMsgBuf = NULL;

	// Remove any new line or extra space characters.
	csErrMsg.TrimRight();
	return csErrMsg;
#else
	return CCoreString(strerror(dwErrorCode));
#endif // WIN32
}

// Get unique file name.
static CCoreString GetUniqueFileName(LPCTSTR pszFilePath = NULL, LPCTSTR pszExtension = NULL)
{
	DWORD dwSize = GetShortPathName(pszFilePath, NULL, 0);
	std::vector<TCHAR> szShortFilePath(dwSize + 1);
	if (GetShortPathName(pszFilePath, &szShortFilePath[0], dwSize)) {
		dwSize = MAX_PATH * 2;
		std::vector<TCHAR> filePath(dwSize);
		if (GetTempFileName(&szShortFilePath[0], _T("RECOVERYxxxxx"), 0, &filePath[0])) {
			CCoreString csFileName = &filePath[0];

			// Delete the created file, we only want the unique file name
			::DeleteFile(csFileName);

			int iPos = csFileName.ReverseFind(_T('.'));
			csFileName = csFileName.Left(iPos + 1);
			csFileName += pszExtension;
			return csFileName;
		}
	}
	return _T("");
}

//
// End of APIs declaration
//

#endif // MG_API_HEADER

//
// End of header.
//
