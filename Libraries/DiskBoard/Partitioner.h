/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for partitions.
 */

#pragma once

#include "AbstractPartitioner.h"

class CAbstractLog;

class CPartitioner : public CAbstractPartitioner
{
public:
	CPartitioner(const CAbstractDiskBoardInterface* pDiskBoard, CAbstractLog* pLog);
	virtual ~CPartitioner();

	virtual BOOL						Initialize();

	/**
	 * Get root of disks/partitions information tree
	 */
	virtual CAbstractPartInfo*			GetDiskItem() const;

private:
	/** Add hard disk info item in tree */
	void								AddHardDiskInfo(CAbstractDisk* pDisk, CAbstractPartInfo** pDiskInfo);

	/** Read partition table, and add info items in tree */
	BOOL								ReadPartitions(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo);

	/** Read GUID partition table, and add info items in tree */
	BOOL	ReadGPTPartitions(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo, CAbstractPartInfo* pInsertAfter);

	/** Print GUID partition table header values */
	void	PrintGUIDHeader(void* pBuffer) const;

private:
	CAbstractLog*						m_pLog;
	CAbstractPartInfo*					m_pPartInfo;
	const CAbstractDiskBoardInterface*	m_pDiskBoard;
};

