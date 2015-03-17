/**
 * Project : Disk Board
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Disk(s)/Partition(s) information tree
 */

#pragma once

#include "AbstractPartInfo.h"

class CAbstractLog;

class CPartInfo : public CAbstractPartInfo
{
public:
	CPartInfo(CAbstractLog* pLog);
	virtual ~CPartInfo();

	// insert/delete info
	virtual CAbstractPartInfo*	Insert(CAbstractPartInfo* pParent, CAbstractPartInfo* pInsertAfter, DWORD dwFlags);

	// iterate on items
	virtual CAbstractPartInfo*	GetParent() const;
	virtual CAbstractPartInfo*	GetChild() const;
	virtual CAbstractPartInfo*	GetNext() const;
	virtual CAbstractPartInfo*	GetPrev() const;

	// set items
	virtual void				SetParent(CAbstractPartInfo* pParent);
	virtual void				SetChild(CAbstractPartInfo* pChild);
	virtual void				SetNext(CAbstractPartInfo* pNext);

	// Get/Set flags
	virtual DWORD				GetFlags() const;
	virtual BOOL				IsFlagExists(DWORD dwFlag) const;
	virtual void				SetFlags(DWORD dwFlag, BOOL bAdd);

	// Get/Set number of sectors
	virtual INT64				GetNumofSectors() const;
	virtual void				SetNumofSectors(INT64 i64Sectors);

	// Get/Set start sector
	virtual INT64				GetStartSector() const;
	virtual void				SetStartSector(INT64 i64Sector);

	// Get/Set Partition Type
	virtual BYTE				GetPartitionType() const;
	virtual void				SetPartitionType(BYTE cType);

private:
	CAbstractLog*				m_pLog;
};

