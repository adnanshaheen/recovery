//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for file menipulation.
//

#ifndef CORE_FILE_HEADER
#define CORE_FILE_HEADER

//
// Decls
//
class CAbstractLog;

//
// headers
//
#include "AbstractFile.h"
#include "CoreString.h"

//
// class
//
class CCoreFile : public CAbstractFile
{
public:
	// Constructor/Destructor
	CCoreFile();
	CCoreFile(CAbstractLog* pLog);
	CCoreFile(const CCoreFile& pFile);
	virtual ~CCoreFile();

	// Create file.
	virtual int		CreateFile();
	virtual int		CreateFile(LPCTSTR pszFileName,				// File name.
								DWORD dwDesiredAccess,			// Desired access.
								DWORD dwSharedMode,				// Sharing mode.
								DWORD dwCreationDisposition,	// Action flags on files that exists.
								DWORD dwFlagsAndAttributes		// File flag attributes.
								);

	// Delete the file.
	virtual int		DeleteFile();
	virtual int		DeleteFile(LPCTSTR pszFileName);

	// Read the file.
	virtual int		ReadFile(void* pBuffer, DWORD dwBytesToRead, DWORD* pBytesRead = nullptr);

	// Read a single line from file.
	virtual int		ReadLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesRead = nullptr);

	// Write the file.
	virtual int		WriteFile(const void* pBuffer, DWORD dwBytesToWrite, DWORD* pBytesWritten = nullptr);

	// Write a single line to file.
	virtual int		WriteLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesWritten = nullptr);

	// Rename the file.
	virtual BOOL	RenameFile(LPCTSTR pszFileName);
	virtual BOOL	RenameFile(LPCTSTR pszOldFileName, LPCTSTR pszNewFileName);

	// Set the end of file.
	virtual BOOL	SetEndOfFile();

	// Check if the file is open.
	virtual BOOL	IsFileOpen();

	// Close the file.
	virtual BOOL	CloseFile();

	// Get/Set file name.
	virtual LPCTSTR	GetFileName();
	virtual void	SetFileName(LPCTSTR pszFileName);

	// Get/Set file path.
	virtual LPCTSTR	GetFilePath();
	virtual void	SetFilePath(LPCTSTR pszFilePath);

	// Get the file attributes.
	virtual BOOL	GetFileAttributes(DWORD& dwFileAttributes);
	virtual BOOL	GetFileAttributes(LPCTSTR pszFileName, DWORD& dwFileAttributes);

	// Set the file attributes.
	virtual BOOL	SetFileAttributes(DWORD dwFileAttributes);
	virtual BOOL	SetFileAttributes(LPCTSTR pszFileName, DWORD dwFileAttributes);

	// Get File pointer.
	virtual BOOL	GetFilePointer(INT64& i64FilePos);

	// Set File pointer.
	virtual BOOL	SetFilePointer(INT64 i64FilePos, DWORD dwMoveMethod = MG_FILE_POS_BEGIN);
	virtual BOOL	SetFilePointer(INT64 i64FilePos, INT64& i64NewPos, DWORD dwMoveMethod = MG_FILE_POS_BEGIN);

	// Get/Set File size.
	virtual BOOL	GetFileSize(INT64& i64FileSize);
	virtual BOOL	GetFileSize(LPCTSTR pszFileName, INT64& i64FileSize);
	virtual BOOL	SetFileSize(INT64 i64FileSize);

	// Get file create, modify, and access date time.
	virtual BOOL	GetCreateDateTime(const CDateTime* pCreateDateTime);
	virtual BOOL	GetModifyDateTime(const CDateTime* pLastAccessDateTime);
	virtual BOOL	GetAccessDateTime(const CDateTime* pLastAccessDateTime);

	// Set file create, modify, and access date time.
	virtual BOOL	SetCreateDateTime(const CDateTime* const pCreateDateTime);
	virtual BOOL	SetModifyDateTime(const CDateTime* const pLastAccessDateTime);
	virtual BOOL	SetAccessDateTime(const CDateTime* const pLastAccessDateTime);

	// Get file handle.
	virtual HANDLE	GetFileHandle() const;

private:
	// Get create/modify/access time.
	BOOL			GetFileTime(FILETIME* const pCreateTime = nullptr,
								FILETIME* const pModifyTime = nullptr,
								FILETIME* const pLastAccessTime = nullptr);

	// Set create/modify/access time.
	BOOL			SetFileTime(const FILETIME* pCreateTime = nullptr,
								const FILETIME* pModifyTime = nullptr,
								const FILETIME* pLastAccessTime = nullptr);

private:
	HANDLE			m_hFile;		// File handle.
	CCoreString		m_csFileName;	// File name.

	CAbstractLog*	m_pLog;			// Log File pointer to write logs
};

//
// end of class
//

#endif // CORE_FILE_HEADER

//
// End of header file.
//
