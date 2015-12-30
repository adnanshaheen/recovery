/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading NT file system.
 */

#pragma once

#include "AbstractFileSystem.h"

class CFileSystemNTFS;
class CAbstractDiskBoardInterface;

class CFileSystem : public CAbstractFileSystem
{
public:
	CFileSystem(CAbstractLog* pLog);
	virtual ~CFileSystem();

	/**
	 * Initialize
	 * Initialize file system
	 * @pPartInfo: Partition to be scanned for meta data and data
	 */
	virtual BOOL Initialize(CAbstractPartInfo* pPartInfo);

	/**
	 * Scan
	 * Read file system meta data, and data
	 * This data will be shown to user, and to be used for recovery
	 */
	virtual BOOL Scan();

	/**
	 * Recover
	 * Start recovering files
	 */
	virtual BOOL Recover();

	/**
	 * Stop
	 * Stop the scan/recovery process
	 */
	virtual BOOL Stop();

protected:
	CAbstractLog* m_pLog;                             /* Log file pointer, to log info */
	CAbstractPartInfo* m_pPartInfo;                   /* Partition Information interface */

private:
	CAbstractDisk* m_pDisk;                           /* Disk interface to read/write on disk */
	CAbstractDiskBoardInterface* m_pThisInterface;    /* This DLL interface to create different objects */
	CFileSystemNTFS* m_pFileSystemNTFS;               /* NTFS File system class pointer */
};
