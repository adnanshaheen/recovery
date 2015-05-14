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
#include "FileSystem.h"

CFileSystem::CFileSystem(CAbstractLog* pLog)
{
}

CFileSystem::~CFileSystem()
{
}

/**
 * Scan
 * Read file system meta data, and data
 * This data will be shown to user, and to be used for recovery
 * @pPartInfo: Partition to be scanned for meta data and data
 */
BOOL CFileSystem::Scan(CAbstractPartInfo* pPartInfo)
{
	ASSERT(FALSE);
	return FALSE;
}

/**
 * Recover
 * Start recovering files
 */
BOOL CFileSystem::Recover()
{
	ASSERT(FALSE);
	return FALSE;
}

/**
 * Stop
 * Stop the scan/recovery process
 */
BOOL CFileSystem::Stop()
{
	ASSERT(FALSE);
	return FALSE;
}
