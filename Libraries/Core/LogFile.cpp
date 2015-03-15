//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for file menipulation.
//

//
// headers
//
#include "Support.h"
#include "LogFile.h"
#include "CoreString.h"
#include "CoreFile.h"
#include "MGAPI.h"

//
// constructor
//
CLogFile::CLogFile()
{
}

//
// copy constructor
//
CLogFile::CLogFile(const CLogFile& pLogFile)
{
	m_pFile = new CCoreFile();
}

//
// destructor
//
/*virtual */CLogFile::~CLogFile()
{
}

//
// Initialize log file, and UI keys.
//
/*virtual */BOOL CLogFile::Initialize(LPCTSTR pszKeyFile, LPCTSTR pszLogFile)
{
	// Create a CoreFile object.
	// with write mode.
	// with name lpszLogFile.
	// with reading data from keys file, if there is any.
	return m_pFile->CreateFile(GetUniqueFileName(),
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_WRITE,
							CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL);
}

//
// Add only a log message.
//
/*virtual */BOOL CLogFile::AddLog(LPCTSTR pszMessage)
{
	return AddLog(pszMessage, NULL);
}

//
// Add log message with file name.
//
/*virtual */BOOL CLogFile::AddLog(LPCTSTR pszMessage, LPCTSTR pszFileName)
{
	if (pszMessage == NULL || _tcslen(pszMessage) <= 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CCoreString csMessage;
	csMessage.Format(_T("%s\nFILE NAME:%s"), pszMessage, pszFileName != NULL ? pszFileName : _T("Unknown"));
	return m_pFile->WriteFile(csMessage, csMessage.GetLength());
}

//
// Add message with icon.
//
/*virtual */BOOL CLogFile::AddLog(LPCTSTR pszMessage, MG_ICON_TYPE eIcon)
{
	return FALSE;
}

//
// Add (nkey) message from a section (having message and icon).
//
/*virtual */BOOL CLogFile::AddLog(UINT nSection, int nKey)
{
	return FALSE;
}

//
// Add (nkey) message from a section (having message and icon) with file name.
//
/*virtual */BOOL CLogFile::AddLog(UINT nSection, int nKey, LPCTSTR pszFileName)
{
	return FALSE;
}

//
// Add (nkey) message from a section (having message and icon) with file name and line number.
//
/*virtual */BOOL CLogFile::AddLog(UINT nSection, int nKey, LPCTSTR pszFileName, UINT uiLineNumber)
{
	return FALSE;
}

//
// Add (nkey) message from a section (having message and icon) with file name and line number with multiple arguments.
//
/*virtual */BOOL CLogFile::AddLog(UINT nSection, int nKey, LPCTSTR pszFileName, UINT uiLineNumber, ...)
{
	return FALSE;
}

//
// Add a message with icon, file name, line number and multiple arguments.
//
/*virtual */BOOL CLogFile::AddLog(LPCTSTR pszMessage, LPCTSTR pszFileName, UINT uiLineNumber, ...)
{
	return FALSE;
}

//
// Add a message with icon, file name, line number and multiple arguments.
//
/*virtual */BOOL CLogFile::AddLog(LPCTSTR pszMessage, MG_ICON_TYPE eIcon, LPCTSTR pszFileName, UINT uiLineNumber, ...)
{
	return FALSE;
}

//
// Move the log file.
//
/*virtual */BOOL CLogFile::MoveLogFile(LPCTSTR pszTargetPath)
{
	return FALSE;
}

//
// Change main log file path.
//
/*virtual */BOOL CLogFile::ChangeMainLogFile(LPCTSTR pszTargetPath)
{
	return FALSE;
}

//
// Append log file to main log.
//
BOOL CLogFile::AppendLogFile()
{
	return FALSE;
}

//
// End of source.
//
