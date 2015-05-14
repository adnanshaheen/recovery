/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading NT file system.
 */

#include "Support.h"
#include "AbstractLog.h"
#include "AbstractDisk.h"
#include "AbstractPartInfo.h"
#include "FileSystemNTFS.h"

CFileSystemNTFS::CFileSystemNTFS()
{
}

CFileSystemNTFS::~CFileSystemNTFS()
{
}

BOOL CFileSystemNTFS::Scan(CAbstractPartInfo* pPartInfo)
{
	ASSERT(FALSE);
	return FALSE;
}

BOOL CFileSystemNTFS::Recover()
{
	ASSERT(FALSE);
	return FALSE;
}

BOOL CFileSystemNTFS::Stop()
{
	ASSERT(FALSE);
	return FALSE;
}
