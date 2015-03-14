//
// Project : 
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for a work status.
//

#ifndef ABSTRACT_STATUS_HEADER
#define ABSTRACT_STATUS_HEADER

//
// class
//
class CAbstractStatus
{
public:
	virtual					~CAbstractStatus() {}

	// Set the progress.
	virtual void			SetProgress(const CAbstractProgress* pProgress) = 0;

	// Set the progress.
	virtual void			SetProgress(const DWORD dwPercent) = 0;

	// Set the file name.
	virtual void			SetFileName(const CAbstractProgress* pProgress) = 0;

	// Set the file name.
	virtual void			SetFileName(LPCTSTR lpszFileName) = 0;

	// Show the dialog.
	virtual LR_DIALOG_RES	ShowDialog(int nDialog, void* pData) = 0;
};

//
// end of class
//

#endif // ABSTRACT_STATUS_HEADER

//
// End of file.
//
