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
#include "FileSystem.h"
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
	if (pDisk == nullptr && pLog != nullptr)
		pLog->AddLog(_T("Error initializing Disk object!!!"), __TFILE__, __LINE__);
	else if (pDisk && pLog)
		pLog->AddLog(_T("Disk initialized!"), __TFILE__, __LINE__);
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
	CAbstractPartitioner* pPartitioner = new CPartitioner(this, pLog);
#ifdef DEBUG
	if (pPartitioner == nullptr && pLog != nullptr)
		pLog->AddLog(_T("Error initializing Partitioner object!!!"), __TFILE__, __LINE__);
	else if (pPartitioner && pLog)
		pLog->AddLog(_T("Partitioner initialized!"), __TFILE__, __LINE__);
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
	if (pPartInfo == nullptr && pLog != nullptr)
		pLog->AddLog(_T("Error initializing PartInfo object!!!"), __TFILE__, __LINE__);
	else if (pPartInfo && pLog)
		pLog->AddLog(_T("Partition Info Tree initialized!"), __TFILE__, __LINE__);
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
		CCoreString csLogFile = fnGetModuleFileName(nullptr);
		csLogFile = csLogFile.Left(csLogFile.ReverseFind(PATH_SEPARATOR));
		pLog->Initialize(nullptr, csLogFile);
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

/**
 * CreateFileSystem
 * Create file system interface
 */
CAbstractFileSystem* CDiskBoardInterface::CreateFileSystem(CAbstractLog* pLog) const
{
	return new CFileSystem(pLog);
}

/**
 * DeleteFileSystem
 * Delete file system interface
 */
void CDiskBoardInterface::DeleteFileSystem(CAbstractFileSystem* pFileSystem)
{
	DELETEME(pFileSystem);
}

//
// End of Source file.
//
