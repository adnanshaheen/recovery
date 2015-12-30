//
// Project : 
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for application log.
//

#ifndef ABSTRACT_LOG_HEADER
#define ABSTRACT_LOG_HEADER

//
// class
//
class CAbstractLog
{
public:
	virtual			~CAbstractLog() {}

	// Initialize log file, and UI keys.
	virtual BOOL	Initialize(LPCTSTR lpszKeyFile, LPCTSTR lpszLogFile) = 0;

	// Add a log message.
	// Add only a log message.
	virtual BOOL	AddLog(LPCTSTR lpszMessage) = 0;

	// Add log message with file name.
	virtual BOOL	AddLog(LPCTSTR lpszMessage, LPCTSTR lpszFileName) = 0;

	// Add log message with file name and line number.
//	virtual BOOL	AddLog(LPCTSTR lpszMessage, LPCTSTR lpszFileName, UINT uiLineNumber) = 0;

	// Add log file in log file as well as UI.
	// Add message with icon.
	virtual BOOL	AddLog(LPCTSTR lpszMessage, MG_ICON_TYPE eIcon) = 0;

	// Add (nkey) message from a section (having message and icon).
	virtual BOOL	AddLog(UINT nSection, int nKey) = 0;

	// Add (nkey) message from a section (having message and icon) with file name.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR lpszFileName) = 0;

	// Add (nkey) message from a section (having message and icon) with file name and line number.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR lpszFileName, UINT uiLineNumber) = 0;

	// Add (nkey) message from a section (having message and icon) with file name and line number with multiple arguments.
	virtual BOOL	AddLog(UINT nSection, int nKey, LPCTSTR lpszFileName, UINT uiLineNumber, ...) = 0;

	// Add a message with file name, line number and multiple arguments.
	virtual BOOL	AddLog(LPCTSTR lpszMessage, LPCTSTR lpszFileName, UINT uiLineNumber, ...) = 0;

	// Add a message with icon, file name, line number and multiple arguments.
	virtual BOOL	AddLog(LPCTSTR lpszMessage, MG_ICON_TYPE eIcon, LPCTSTR lpszFileName, UINT uiLineNumber, ...) = 0;

	// Move the log file.
	virtual BOOL	MoveLogFile(LPCTSTR lpszTargetPath) = 0;

	// Change main log file path.
	virtual BOOL	ChangeMainLogFile(LPCTSTR lpszTargetPath) = 0;

	virtual BOOL	AddDebugLog(LPCTSTR lpszMessage, LPCTSTR lpszFileName, UINT uiLineNumber, ...) = 0;
};

//
// end of class
//

#endif // ABSTRACT_LOG_HEADER

//
// End of file.
//
