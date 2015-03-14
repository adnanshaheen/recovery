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
CCoreString GetErrorMessage(DWORD dwErrorCode);

// Get unique file name.
CCoreString GetUniqueFileName(LPCTSTR pszFilePath = NULL, LPCTSTR pszExtension = NULL);
//
// End of APIs declaration
//

#endif // MG_API_HEADER

//
// End of header.
//
