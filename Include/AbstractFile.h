//
// Project : 
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for file menipulation.
//

#ifndef ABSTRACT_FILE_HEADER
#define ABSTRACT_FILE_HEADER

//
// forward class decls
//
class CDateTime;

//
// class
//
class CAbstractFile
{
public:
	virtual			~CAbstractFile() {}

	// Create file.
	virtual int		CreateFile() = 0;
	virtual int		CreateFile(LPCTSTR pszFileName,							// File name.
								DWORD dwDesiredAccess,						// Desired access.
								DWORD dwSharedMode,							// Sharing mode.
								DWORD dwCreationDisposition,				// Action flags on files that exists.
								DWORD dwFlagsAndAttributes					// File flag attributes.
								) = 0;
	// Delete the file.
	virtual int		DeleteFile() = 0;
	virtual int		DeleteFile(LPCTSTR pszFileName) = 0;

	// Read the file.
	virtual int		ReadFile(void* pBuffer, DWORD dwBytesToRead, DWORD* pBytesRead = NULL) = 0;
	virtual int		ReadFile(LPCTSTR lpszFileName, void* pBuffer, DWORD dwBytesToRead, DWORD* pBytesRead = NULL) = 0;

	// Read a single line from file.
	virtual int		ReadLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesRead = NULL) = 0;

	// Write the file.
	virtual int		WriteFile(const void* pBuffer, DWORD dwBytesToWrite, DWORD* pBytesWritten = NULL) = 0;
	virtual int		WriteFile(LPCTSTR pszFileName, const void* pBuffer, DWORD dwBytesToWrite, DWORD* pBytesWritten = NULL) = 0;

	// Write a single line to file.
	virtual int		WriteLine(std::basic_string<TCHAR>& cLine, DWORD* pBytesWritten = NULL) = 0;

	// Rename the file.
	virtual BOOL	RenameFile(LPCTSTR pszFileName) = 0;
	virtual BOOL	RenameFile(LPCTSTR pszOldFileName, LPCTSTR pszNewFileName) = 0;

	// Set the end of file.
	virtual BOOL	SetEndOfFile() = 0;

	// Check if the file is open.
	virtual BOOL	IsFileOpen() = 0;

	// Close the file.
	virtual BOOL	CloseFile() = 0;

	// Get/Set file name.
	virtual LPCTSTR	GetFileName() = 0;
	virtual void	SetFileName(LPCTSTR pszFileName) = 0;

	// Get/Set file path.
	virtual LPCTSTR	GetFilePath() = 0;
	virtual void	SetFilePath(LPCTSTR pszFilePath) = 0;

	// Get the file attributes.
	virtual BOOL	GetFileAttributes(DWORD& dwFileAttributes) = 0;
	virtual BOOL	GetFileAttributes(LPCTSTR pszFileName, DWORD& dwFileAttributes) = 0;

	// Set the file attributes.
	virtual BOOL	SetFileAttributes(DWORD dwFileAttributes) = 0;
	virtual BOOL	SetFileAttributes(LPCTSTR pszFileName, DWORD dwFileAttributes) = 0;

	// Get File pointer.
	virtual BOOL	GetFilePointer(INT64& i64FilePos) = 0;

	// Set File pointer.
	virtual BOOL	SetFilePointer(INT64 i64FilePos, DWORD dwMoveMethod = MG_FILE_POS_BEGIN) = 0;
	virtual BOOL	SetFilePointer(INT64 i64FilePos, INT64& i64NewPos, DWORD dwMoveMethod = MG_FILE_POS_BEGIN) = 0;

	// Get/Set File size.
	virtual BOOL	GetFileSize(INT64& i64FileSize) = 0;
	virtual BOOL	GetFileSize(LPCTSTR lpszFileName, INT64& i64FileSize) = 0;
	virtual BOOL	SetFileSize(INT64 i64FileSize) = 0;

	// Get file create, modify, and access date time.
	virtual BOOL	GetCreateDateTime(const CDateTime* pCreateDateTime) = 0;
	virtual BOOL	GetModifyDateTime(const CDateTime* pLastAccessDateTime) = 0;
	virtual BOOL	GetAccessDateTime(const CDateTime* pLastAccessDateTime) = 0;

	// Set file create, modify, and access date time.
	virtual BOOL	SetCreateDateTime(const CDateTime* const pCreateDateTime) = 0;
	virtual BOOL	SetModifyDateTime(const CDateTime* const pLastAccessDateTime) = 0;
	virtual BOOL	SetAccessDateTime(const CDateTime* const pLastAccessDateTime) = 0;

	// Get file handle.
	virtual HANDLE	GetFileHandle() const = 0;
};

//
// end of class
//

#endif // ABSTRACT_FILE_HEADER

//
// End of file.
//
