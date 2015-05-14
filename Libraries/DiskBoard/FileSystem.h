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

class CFileSystem : public CAbstractFileSystem
{
public:
	CFileSystem(CAbstractLog* pLog);
	virtual ~CFileSystem();

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
