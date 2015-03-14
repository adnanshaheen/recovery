//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for file menipulation.
//

//
// headers
//
//#include <WinDef.h>
#include <string>
#include "Types.h"
#include "CoreFile.h"
#include "MGAPI.h"
#include "AbstractLog.h"
#include "DateTime.h"

//
// Constructor
//
CCoreFile::CCoreFile()
{
}
CCoreFile::CCoreFile(CAbstractLog *pLog)
{
	m_pLog = pLog;
}

//
// Copy Constructor
//
CCoreFile::CCoreFile(const CCoreFile& pFile)
{
}

//
// Destructor
//
/*virtual */CCoreFile::~CCoreFile()
{
}

//
// Create file.
//
/*virtual */int CCoreFile::CreateFile()
{
	CCoreString csFileName = GetUniqueFileName();

	// Create the file with default values.
	return this->CreateFile(csFileName,
							0,
							0,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_TEMPORARY);
}
/*virtual */int CCoreFile::CreateFile(LPCTSTR pszFileName,			// File name.
									DWORD dwDesiredAccess,			// Desired access.
									DWORD dwSharedMode,				// Sharing mode.
									DWORD dwCreationDisposition,	// Action flags on files that exists.
									DWORD dwFlagsAndAttributes		// File flag attributes.
									)
{
	// Set the path and file name.
	CCoreString csFileName;
	if (pszFileName == NULL || _tcslen(pszFileName) == 0)
	{
		csFileName = GetUniqueFileName();
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("File name not specified, a file with %s name."), __TFILE__, __LINE__, csFileName);

	}
	else
	{
		csFileName = pszFileName;
	}

	// Make sure the file is not opened earlier.
	this->CloseFile();

	// Save the file path and name.
	this->SetFilePath(csFileName);

	// File handle must be invalid at this point.
	ASSERT(m_hFile == NULL || m_hFile == INVALID_HANDLE_VALUE);

	// Call the WINAPI to Create file.
	m_hFile = ::CreateFile(csFileName, dwDesiredAccess, dwSharedMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);

	// File handle must be valid now.
	ASSERT(m_hFile != NULL || m_hFile != INVALID_HANDLE_VALUE);

	// Set the file handle correctly.
	m_hFile = m_hFile != NULL || m_hFile != INVALID_HANDLE_VALUE ? m_hFile : NULL;
	if (m_hFile == NULL)
	{
		// Log the errors, file could not be created.
		if (m_pLog != NULL)
		{
			m_pLog->AddLog(_T("File %s could not be created due to %s."), __TFILE__, __LINE__,
							csFileName, GetErrorMessage(::GetLastError()));
		}
	}

	// Return the result.
	return m_hFile != NULL ? 0 : -1;
}

//
// Delete the file.
//
/*virtual */int CCoreFile::DeleteFile()
{
	if (m_csFileName.IsEmpty())
	{
		// Nothing to delete, simply return succuess.
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("Nothing to delete!!!"), __TFILE__, __LINE__);

		return 0;
	}

	return this->DeleteFile(this->GetFilePath());
}
/*virtual */int CCoreFile::DeleteFile(LPCTSTR pszFileName)
{
	if (pszFileName == NULL)
	{
		// Log the error, file name not available.
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("File name not available to delete!!!"), __TFILE__, __LINE__);

		// Return failure.
		return -1;
	}

	if (!::DeleteFile(pszFileName))
	{
		// Log the error, file could not be deleted.
		if (m_pLog != NULL)
		{
			m_pLog->AddLog(_T("File %s could not be deleted due to %s."), __TFILE__, __LINE__,
							pszFileName, GetErrorMessage(::GetLastError()));
		}

		// Return failure.
		return -1;
	}

	return 0;
}

//
// Read the file.
//
/*virtual */int CCoreFile::ReadFile(void* pBuffer, DWORD dwBytesToRead, DWORD* pBytesRead/* = NULL*/)
{
	if (pBuffer == NULL || dwBytesToRead == 0)
	{
		// Log error message, invalid buffer.
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("Invalid parameters to read file!!!"), __TFILE__, __LINE__);

		// Return failure.
		return -1;
	}

	if (!this->IsFileOpen())
	{
		// Log error message, file is not open.
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("File is not opened to read!!!"), __TFILE__, __LINE__);

		// Return failure.
		return -1;
	}

	DWORD dwBytesRead = 0;
	int iRes = ::ReadFile(m_hFile, pBuffer, dwBytesToRead, &dwBytesRead, NULL);
	if (pBytesRead != NULL)
		pBytesRead = &dwBytesRead;

	if (iRes <= 0)
	{
		// Log error message, file could not be read.
		if (m_pLog != NULL)
		{
			m_pLog->AddLog(_T("File %s could not be read due to %s.!!!"), __TFILE__, __LINE__,
							m_csFileName, GetErrorMessage(::GetLastError()));
		}

		return -1;
	}

	return 0;
}
/*virtual */int CCoreFile::ReadFile(LPCTSTR pszFileName, BYTE* pBuffer, DWORD dwBytesToRead, DWORD* pBytesRead/* = NULL*/)
{
	// Open the file pszFileName.
	// Read the file.

	int iRes = 0;
	iRes = this->CreateFile(pszFileName, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY);

	if (iRes > 0)
		iRes = this->ReadFile(pBuffer, dwBytesToRead, pBytesRead);

	return iRes;
}

//
// Read a single line from file.
//
/*virtual */int CCoreFile::ReadLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesRead/* = NULL*/)
{
	// First read the file normally, then find out the first line and truncate rest of buffer.
	// This method will read the file from current position.
	int iRes = 0;

	const int kBufferSize = 25;
	TCHAR cBuffer[kBufferSize] = { 0 };
	TCHAR* pNewLine = _T("\n");
	DWORD dwTotalBytesRead = 0;
	do
	{
		DWORD dwBytesRead = 0;
		iRes = this->ReadFile(&cBuffer, kBufferSize, &dwBytesRead);

		TCHAR* pEndLine = _tcsstr(cBuffer, pNewLine);
		if (pEndLine != NULL)
		{
			*pEndLine = _T('\0');
			INT64 i64FilePos = 0;
			this->GetFilePointer(i64FilePos);
			this->SetFilePointer(i64FilePos - dwBytesRead);
			iRes = MG_END_OF_LINE;
		}

		dwTotalBytesRead += dwBytesRead;
		cLine.append(cBuffer);
	}
	while (iRes != MG_END_OF_LINE);

	if (pBytesRead != NULL)
		pBytesRead = &dwTotalBytesRead;

	return iRes == MG_END_OF_LINE ? 0 : -1;
}

//
// Write the file.
//
/*virtual */int CCoreFile::WriteFile(const void* pBuffer, DWORD dwBytesToWrite, DWORD* pBytesWritten/* = NULL*/)
{
	if (pBuffer == NULL || dwBytesToWrite == 0)
	{
		// Log error message, invalid buffer.
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("Invalid parameters to write file!!!"), __TFILE__, __LINE__);

		// Return failure.
		return -1;
	}

	int iRes = 0;
	try
	{
		if (!this->IsFileOpen())
		{
			// Log error message, file is not open.
			if (m_pLog != NULL)
				m_pLog->AddLog(_T("File is not opened to write!!!"), __TFILE__, __LINE__);

			if (m_csFileName.IsEmpty())
			{
				iRes = this->CreateFile();
				if (iRes <= -1)
					throw iRes;

				// Log info message, temp file to be opened.
				if (m_pLog != NULL)
					m_pLog->AddLog(_T("File %s is created for writing."), __TFILE__, __LINE__, m_csFileName);
			}
			else
			{
				// The file must be open.
			}
		}
		else
		{
			// File is open, go for writing.
		}

		DWORD dwBytesWritten = 0;
		iRes = ::WriteFile(m_hFile, pBuffer, dwBytesToWrite, &dwBytesWritten, NULL);
		if (pBytesWritten != NULL)
			pBytesWritten = &dwBytesWritten;

		if (iRes <= 0)
		{
			// Log the error, file could not be written.
			if (m_pLog != NULL)
			{
				m_pLog->AddLog(_T("Could not write file %sdue to %s!!!"), __TFILE__, __LINE__,
								m_csFileName, GetErrorMessage(::GetLastError()));
			}
		}
	}
	catch (int)
	{
	}

	return iRes;
}
/*virtual */int CCoreFile::WriteFile(LPCTSTR pszFileName, const BYTE* pBuffer, DWORD dwBytesToWrite, DWORD* pBytesWritten/* = NULL*/)
{
	int iRes = this->CreateFile(pszFileName, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY);

	if (iRes >= 0)
		iRes = this->WriteFile(pBuffer, dwBytesToWrite, pBytesWritten);

	return iRes;
}

//
// Write a single line to file.
//
/*virtual */int CCoreFile::WriteLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesWritten/* = NULL*/)
{
	int iRes = 0;
	try
	{
		if (!this->IsFileOpen())
		{
			// Open the file for writing.
			if (m_csFileName.IsEmpty())
			{
				iRes = this->CreateFile();
				if (iRes <= -1)
					throw iRes;
			}
			else
			{
				// The file must be opened.
			}
		}
		else
		{
			// File is open, go for writing the line.
		}

		CCoreString csLine = cLine.c_str();
		csLine.TrimRight(_T("\n"));
		csLine += _T("\n");

		// Line is ready, now write it.
		DWORD dwBytesWritten = 0;
		iRes = this->WriteFile((LPCTSTR) csLine, csLine.GetLength(), &dwBytesWritten);
		if (pBytesWritten != NULL)
			pBytesWritten = &dwBytesWritten;
	}
	catch (int)
	{
	}

	return iRes;
}

//
// Rename the file.
//
/*virtual */BOOL CCoreFile::RenameFile(LPCTSTR pszFileName)
{
	return this->RenameFile(m_csFileName, pszFileName);
}
/*virtual */BOOL CCoreFile::RenameFile(LPCTSTR pszOldFileName, LPCTSTR pszNewFileName)
{
	// Open the pszOldFileName in read mode.
	// Open the pszNewFileName in write mode.
	// copy contents.
	// Delete pszOldFileName.

	int nRes = 0;
	try
	{
		nRes = this->CreateFile(pszOldFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN);
		if (nRes < 0)
		{
			ASSERT(FALSE);
			throw nRes;
		}

		nRes = this->CreateFile(pszNewFileName, GENERIC_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
		if (nRes < 0)
		{
			ASSERT(FALSE);
			throw nRes;
		}

		INT64 i64FileSize = 0;
		this->GetFileSize(i64FileSize);

		if (i64FileSize == 0)
		{
			// Just delete this file.
			nRes = this->DeleteFile();
			throw nRes;
		}

		std::vector<BYTE> cBuffer(FILE_BUFFER_SIZE);
		while (i64FileSize > 0)
		{
			DWORD dwBytesToRead = FILE_BUFFER_SIZE;
			if (FILE_BUFFER_SIZE > i64FileSize)
				dwBytesToRead = static_cast<DWORD> (i64FileSize);

			DWORD dwBytesRead = 0;
			nRes = this->ReadFile(&cBuffer[0], dwBytesToRead, &dwBytesRead);
			if (nRes < 0)
				break;

			if (dwBytesRead != dwBytesToRead)
			{
				nRes = -1;
				throw nRes;
			}

			i64FileSize -= dwBytesRead;

			DWORD dwBytesWritten = 0;
			nRes = this->WriteFile(&cBuffer[0], dwBytesRead, &dwBytesWritten);
			if (nRes < 0)
				break;
		}
	}
	catch (int)
	{
	}

	return nRes;
}

//
// Set the end of file.
//
/*virtual */BOOL CCoreFile::SetEndOfFile()
{
	if (!this->IsFileOpen())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRes = ::SetEndOfFile(m_hFile);
	if (!bRes && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("End of file for file %s could not be set due to %s!!!"), __TFILE__, __LINE__,
						m_csFileName, GetErrorMessage(::GetLastError()));
	}

	return bRes;
}

//
// Check if the file is open.
//
/*virtual */BOOL CCoreFile::IsFileOpen()
{
	return m_hFile != NULL ? TRUE : FALSE;
}

//
// Close the file.
//
/*virtual */BOOL CCoreFile::CloseFile()
{
	if (this->IsFileOpen())
	{
		// No file is opened. So return success.
		return TRUE;
	}

	BOOL bRes = ::CloseHandle(m_hFile);
	if (!bRes && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s could not be closed due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));
	}

	return bRes;
}

//
// Get/Set file name.
//
/*virtual */LPCTSTR CCoreFile::GetFileName()
{
	return this->m_csFileName;
}
/*virtual */void CCoreFile::SetFileName(LPCTSTR pszFileName)
{
	this->m_csFileName = pszFileName;
}

//
// Get/Set file path.
//
/*virtual */LPCTSTR CCoreFile::GetFilePath()
{
	return this->GetFileName();
}
/*virtual */void CCoreFile::SetFilePath(LPCTSTR pszFilePath)
{
	this->SetFileName(pszFilePath);
}

//
// Get the file attributes.
//
/*virtual */BOOL CCoreFile::GetFileAttributes(DWORD& dwFileAttributes)
{
	dwFileAttributes = MG_INVALID_FILE;
	if (m_csFileName.IsEmpty())
	{
		// We don't have an open file.
		// Return the error.
		ASSERT(FALSE);
		return FALSE;
	}

	return this->GetFileAttributes(m_csFileName, dwFileAttributes);
}
/*virtual */BOOL CCoreFile::GetFileAttributes(LPCTSTR pszFileName, DWORD& dwFileAttributes)
{
	if (pszFileName == NULL || _tcslen(pszFileName) == 0)
	{
		// File not not valid.
		// Return with error.
		ASSERT(FALSE);
		return FALSE;
	}

	dwFileAttributes = ::GetFileAttributes(pszFileName);
	if (dwFileAttributes == MG_INVALID_FILE && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s attributes could not be retrieved due to %s!!!"), __TFILE__, __LINE__,
			pszFileName, GetErrorMessage(::GetLastError()));

		return FALSE;
	}

	return TRUE;
}

//
// Set the file attributes.
//
/*virtual */BOOL CCoreFile::SetFileAttributes(DWORD dwFileAttributes)
{
	if (m_csFileName.IsEmpty())
	{
		// We don't have an open file.
		// Return the error.
		ASSERT(FALSE);
		return FALSE;
	}

	return this->SetFileAttributes(m_csFileName, dwFileAttributes);
}
/*virtual */BOOL CCoreFile::SetFileAttributes(LPCTSTR pszFileName, DWORD dwFileAttributes)
{
	if (pszFileName == NULL || _tcslen(pszFileName) == 0)
	{
		// File not not valid.
		// Return with error.
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRes = ::SetFileAttributes(pszFileName, dwFileAttributes);
	if (!bRes && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s attributes could not be set due to %s!!!"), __TFILE__, __LINE__,
			pszFileName, GetErrorMessage(::GetLastError()));
	}

	return bRes;
}

//
// Get File pointer.
//
/*virtual */BOOL CCoreFile::GetFilePointer(INT64& i64FilePos)
{
	if (!this->IsFileOpen())
	{
		// File is not open.
		// Return with error.
		ASSERT(FALSE);
		return FALSE;
	}

	i64FilePos = 0;
	LONG nLow = 0;
	LONG nHigh = 0;
	nLow = ::SetFilePointer(m_hFile, 0, &nHigh, MG_FILE_POS_CURRENT);
	if (nLow == MG_FILE_POINTER_INVALID && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s position could not be retrieved due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));

		return FALSE;
	}

	i64FilePos = (static_cast<INT64> (nHigh) << 32) + nLow;
	return TRUE;
}

//
// Set File pointer.
//
/*virtual */BOOL CCoreFile::SetFilePointer(INT64 i64FilePos, MG_FILE_MOVE eMoveMethod/* = MG_FILE_POS_BEGIN*/)
{
	INT64 i64NewPos = 0;
	return SetFilePointer(i64FilePos, i64NewPos, eMoveMethod);
}
/*virtual */BOOL CCoreFile::SetFilePointer(INT64 i64FilePos, INT64& i64NewPos, MG_FILE_MOVE eMoveMethod/* = MG_FILE_POS_BEGIN*/)
{
		if (!this->IsFileOpen())
	{
		// File is not open.
		// Return with error.
		ASSERT(FALSE);
		return FALSE;
	}

	LONG nLow = 0;
	LONG nHigh = 0;
	nLow = static_cast<LONG> (i64FilePos);
	nHigh = static_cast<LONG> (i64FilePos >> 32);
	DWORD dwLow = ::SetFilePointer(m_hFile, nLow, &nHigh, eMoveMethod);
	if (dwLow == MG_FILE_POINTER_INVALID && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s position could not be set due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));

		return FALSE;
	}

	i64NewPos = ((INT64) nHigh << 32) + dwLow;

	return TRUE;
}

//
// Get/Set File size.
//
/*virtual */BOOL CCoreFile::GetFileSize(INT64& i64FileSize)
{
	if (!this->IsFileOpen())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwLow = 0;
	DWORD dwHigh = 0;
	dwLow = ::GetFileSize(m_hFile, &dwHigh);
	if (dwLow == MG_FILE_POINTER_INVALID && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s size could not be retrieved due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));

		return FALSE;
	}

	return TRUE;
}
/*virtual */BOOL CCoreFile::GetFileSize(LPCTSTR pszFileName, INT64& i64FileSize)
{
	// TODO: impliment this method
	ASSERT(FALSE);
	return FALSE;
}
/*virtual */BOOL CCoreFile::SetFileSize(INT64 i64FileSize)
{
	// TODO: impliment this method
	ASSERT(FALSE);
	return FALSE;
}

//
// Get file create, modify, and access date time.
//
/*virtual */BOOL CCoreFile::GetCreateDateTime(const CDateTime* pCreateDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = this->GetFileTime(&cFileTime);
	CDateTime cCreateDateTime(cFileTime);
	pCreateDateTime = &cCreateDateTime;
	return bRes;
}
/*virtual */BOOL CCoreFile::GetModifyDateTime(const CDateTime* pLastAccessDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = this->GetFileTime(NULL, &cFileTime);
	CDateTime cLastAccessDateTime(cFileTime);
	pLastAccessDateTime = &cLastAccessDateTime;
	return bRes;
}
/*virtual */BOOL CCoreFile::GetAccessDateTime(const CDateTime* pLastAccessDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = this->GetFileTime(NULL, NULL, &cFileTime);
	CDateTime cLastAccessDateTime(cFileTime);
	pLastAccessDateTime = &cLastAccessDateTime;
	return bRes;
}
BOOL CCoreFile::GetFileTime(FILETIME* const pCreateTime/* = NULL*/,
							FILETIME* const pModifyTime/* = NULL*/,
							FILETIME* const pLastAccessTime/* = NULL*/)
{
	BOOL bRes = ::GetFileTime(m_hFile, pCreateTime, pModifyTime, pLastAccessTime);
	if (!bRes && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s date/time could not be retrieved due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));
	}

	return bRes;
}

//
// Set file create, modify, and access date time.
//
/*virtual */BOOL CCoreFile::SetCreateDateTime(const CDateTime* pCreateDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = pCreateDateTime->GetAsFileTime(cFileTime);
	bRes |= this->SetFileTime(&cFileTime);
	return bRes;
}
/*virtual */BOOL CCoreFile::SetModifyDateTime(const CDateTime* pLastAccessDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = pLastAccessDateTime->GetAsFileTime(cFileTime);
	bRes |= this->SetFileTime(NULL, &cFileTime);
	return bRes;
}
/*virtual */BOOL CCoreFile::SetAccessDateTime(const CDateTime* pLastAccessDateTime)
{
	FILETIME cFileTime;
	BOOL bRes = pLastAccessDateTime->GetAsFileTime(cFileTime);
	bRes |= this->SetFileTime(NULL, NULL, &cFileTime);
	return bRes;
}
BOOL CCoreFile::SetFileTime(const FILETIME* pCreateTime/* = NULL*/,
							const FILETIME* pModifyTime/* = NULL*/,
							const FILETIME* pLastAccessTime/* = NULL*/)
{
	BOOL bRes = ::SetFileTime(m_hFile, pCreateTime, pModifyTime, pLastAccessTime);
	if (!bRes && m_pLog != NULL)
	{
		m_pLog->AddLog(_T("File %s date/time could not be set due to %s!!!"), __TFILE__, __LINE__,
			m_csFileName, GetErrorMessage(::GetLastError()));
	}

	return bRes;
}

//
// Get file handle
//
HANDLE CCoreFile::GetFileHandle() const
{
	return m_hFile;
}

//
// End of source file.
//
