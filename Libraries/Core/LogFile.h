//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for file menipulation.
//

#ifndef LOG_FILE_HEADER
#define LOG_FILE_HEADER

//
// header
//
#include "AbstractFile.h"

//
// class
//
class CLogFile : private CAbstractFile
{
public:
	// Constructor/Destructor
	CLogFile();
	CLogFile(const CLogFile& pLogFile);
	virtual ~CLogFile();

	// Initialize log file, and UI keys.
	virtual BOOL	Initialize(LPCTSTR pszKeyFile, LPCTSTR pszLogFile);

	// Add a log message.
	// Add only a log message.
	virtual BOOL	AddLog(LPCTSTR pszMessage);

	// Add log message with file name.
	virtual BOOL	AddLog(LPCTSTR pszMessage, LPCTSTR pszFileName);

	// Add log file in log file as well as UI.
	// Add message with icon.
	virtual BOOL	AddLog(LPCTSTR pszMessage, MG_ICON_TYPE eIcon);

	// Add (nkey) message from a section (having message and icon).
	virtual BOOL	AddLog(UINT nSection, int nKey);

	// Add (nkey) message from a section (having message and icon) with file name.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR pszFileName);

	// Add (nkey) message from a section (having message and icon) with file name and line number.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR pszFileName, UINT uiLineNumber);

	// Add (nkey) message from a section (having message and icon) with file name and line number with multiple arguments.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR pszFileName, UINT uiLineNumber, ...);

	// Add a message with file name, line number and multiple arguments.
	virtual BOOL	AddLog(LPCTSTR pszMessage, LPCTSTR pszFileName, UINT uiLineNumber, ...);

	// Add a message with icon, file name, line number and multiple arguments.
	virtual BOOL	AddLog(LPCTSTR pszMessage, MG_ICON_TYPE eIcon, LPCTSTR pszFileName, UINT uiLineNumber, ...);

	// Move the log file.
	virtual BOOL	MoveLogFile(LPCTSTR pszTargetPath);

	// Change main log file path.
	virtual BOOL	ChangeMainLogFile(LPCTSTR pszTargetPath);

private:
	// Append log file to main log.
	BOOL			AppendLogFile();
};

//
// end of class.
//

#endif // LOG_FILE_HEADER

//
// End of header.
//
