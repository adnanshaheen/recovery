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

private:
	void								AddHardDiskInfo(CAbstractDisk* pDisk, CAbstractPartInfo** pDiskInfo);

private:
	CAbstractLog*						m_pLog;
	CAbstractPartInfo*					m_pPartInfo;
	const CAbstractDiskBoardInterface*	m_pDiskBoard;
};

