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
class CPartInfoData;

class CPartInfo : public CAbstractPartInfo
{
public:
	CPartInfo(CAbstractLog* pLog);
	virtual ~CPartInfo();

	/**
	 * Insert PartInfo
	 * @pParent : parent node
	 * @pInsertAfter : insert item after this item
	 * @dwFlags: flags
	 */
	virtual CAbstractPartInfo*	Insert(CAbstractPartInfo* pParent, CAbstractPartInfo* pInsertAfter, DWORD dwFlags);

	// iterate on items
	virtual CAbstractPartInfo*	GetParent() const;
	virtual CAbstractPartInfo*	GetChild() const;
	virtual CAbstractPartInfo*	GetNext() const;

	// set items
	virtual void				SetParent(CAbstractPartInfo* pParent);
	virtual void				SetChild(CAbstractPartInfo* pChild);
	virtual void				SetNext(CAbstractPartInfo* pNext);

	// Get/Set flags
	virtual DWORD				GetFlags() const;
	virtual BOOL				IsFlagExists(DWORD dwFlag) const;
	virtual void				SetFlags(DWORD dwFlag, BOOL bAdd);

	// Get/Set number of sectors
	virtual INT64				GetSectors() const;
	virtual void				SetSectors(INT64 i64Sectors);

	/**
	 * Get start sector
	 */
	virtual INT64				GetStartSector() const;
	/**
	 * Set start sector
	 *
	 * @i64Sector : sector number
	 */
	virtual void				SetStartSector(INT64 i64Sector);

	// Get/Set Partition Type
	virtual BYTE				GetPartitionType() const;
	virtual void				SetPartitionType(BYTE cType);

	/**
	 * Get partition type GUID
	 */
	virtual GUID				GetPartitionTypeGUID() const;
	/**
	 * Set partition type GUID
	 */
	virtual void				SetPartitionTypeGUID(GUID Type);

	/**
	 * Get disk number
	 */
	virtual int					GetDiskNumber() const;
	/**
	 * Set disk number
	 * param
	 * @nDiskNumber : disk number to set
	 */
	virtual void				SetDiskNumber(int nDiskNumber);

	/**
	 * Get partition number
	 */
	virtual int					GetPartitionNumber() const;
	/**
	 * Set partition number
	 * param
	 * @nPartitionNumber : partition number to set
	 */
	virtual void				SetPartitionNumber(int nPartitionNumber);

private:
	CAbstractLog*				m_pLog;
	CPartInfoData*				m_pPartInfoData;
};

