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

CFileSystemNTFS::CFileSystemNTFS() : CFileSystem(m_pLog)
{
}

CFileSystemNTFS::~CFileSystemNTFS()
{
}

/**
 * Scan
 * Read file system meta data, and data
 * This data will be shown to user, and to be used for recovery
 */
BOOL CFileSystemNTFS::Scan()
{
	ASSERT(FALSE);
	return FALSE;
}

/**
 * Recover
 * Start recovering files
 */
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
