//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for Disk Board DLL.
//

#include "Types.h"
#include "DiskBoardInterface.h"
#include "Disk.h"
#include "AbstractLog.h"

//
// Constructor
//
CDiskBoardInterface::CDiskBoardInterface()
{
}

//
// Destructor
//
CDiskBoardInterface::~CDiskBoardInterface()
{
}

//
// Create CAbstractDisk
//
CAbstractDisk* CDiskBoardInterface::CreateDisk(CAbstractLog *pLog) const
{
	CAbstractDisk* pDisk = new CDisk(pLog);
#ifdef DEBUG
	if (pDisk == NULL && pLog != NULL)
		pLog->AddLog(_T("Error initializing Disk object!!!"), __TFILE__, __LINE__);
#endif // DEBUG
	return pDisk;
}

//
// Delete AbstractDisk
//
void CDiskBoardInterface::DeleteDisk(CAbstractDisk* pDisk)
{
	if (pDisk != NULL)
	{
		delete pDisk;
		pDisk = NULL;
	}
}

//
// End of Source file.
//
