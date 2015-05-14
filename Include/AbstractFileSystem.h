/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading file system.
 */

#pragma once

#include "Support.h"

class CAbstractPartInfo;

class CAbstractFileSystem
{
public:
	virtual ~CAbstractFileSystem() {}

	/**
	 * Initialize
	 * Initialize file system
	 * @pPartInfo: Partition to be scanned for meta data and data
	 */
	virtual BOOL Initialize(CAbstractPartInfo* pPartInfo) = 0;

	/**
	 * Scan
	 * Read file system meta data, and data
	 * This data will be shown to user, and to be used for recovery
	 */
	virtual BOOL Scan() = 0;

	/**
	 * Recover
	 * Start recovering files
	 */
	virtual BOOL Recover() = 0;

	/**
	 * Stop
	 * Stop the scan/recovery process
	 */
	virtual BOOL Stop() = 0;
};
