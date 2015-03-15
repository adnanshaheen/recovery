//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for Disk Board DLL.
//

#include "Support.h"
#include "DiskBoardInterface.h"
#include "Disk.h"
#include "AbstractLog.h"
#include "LogFile.h"
#include "PartInfo.h"
#include "Partitioner.h"
#include "CoreString.h"
#include "MGAPI.h"

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
	DELETEME(pDisk);
}

//
// Create Partitioner
//
CAbstractPartitioner* CDiskBoardInterface::CreatePartitioner(CAbstractLog* pLog) const
{
	CAbstractPartitioner* pPartitioner = new CPartitioner(pLog);
#ifdef DEBUG
	if (pPartitioner == NULL && pLog != NULL)
		pLog->AddLog(_T("Error initializing Partitioner object!!!"), __TFILE__, __LINE__);
#endif // DEBUG
	return pPartitioner;
}

//
// Delete partitioner
//
void CDiskBoardInterface::DeletePartitioner(CAbstractPartitioner* pPartitioner)
{
	DELETEME(pPartitioner);
}

//
// Create Partition Info
//
CAbstractPartInfo* CDiskBoardInterface::CreatePartInfo(CAbstractLog* pLog) const
{
	CAbstractPartInfo* pPartInfo = new CPartInfo(pLog);
#ifdef DEBUG
	if (pPartInfo == NULL && pLog != NULL)
		pLog->AddLog(_T("Error initializing PartInfo object!!!"), __TFILE__, __LINE__);
#endif // DEBUG
	return pPartInfo;
}

//
// Delete partition info
//
void CDiskBoardInterface::DeletePartInfo(CAbstractPartInfo* pPartInfo)
{
	DELETEME(pPartInfo);
}

//
// Create log
//
CAbstractLog* CDiskBoardInterface::CreateLogFile() const
{
	CAbstractLog* pLog = new CLogFile();
	if (pLog) {
		CCoreString csLogFile = fnGetModuleFileName(NULL);
		csLogFile = csLogFile.Left(csLogFile.ReverseFind(PATH_SEPARATOR));
		pLog->Initialize(NULL, csLogFile);
	}

	return pLog;
}

//
// Delete log
//
void CDiskBoardInterface::DeleteLogFile(CAbstractLog* pLog)
{
	DELETEME(pLog);
}

//
// End of Source file.
//
