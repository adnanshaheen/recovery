/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading NT file system.
 */

#pragma once

#include "FileSystem.h"

class CFileSystemNTFS : public CAbstractFileSystem
{
public:
	CFileSystemNTFS();
	virtual ~CFileSystemNTFS();

	/**
	 * Scan
	 * Read file system meta data, and data
	 * This data will be shown to user, and to be used for recovery
	 * @pPartInfo: Partition to be scanned for meta data and data
	 */
	virtual BOOL Scan(CAbstractPartInfo* pPartInfo);

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
};
