//
// Project : recovery
//
// Author : Adnan Shaheen
//
//
// Purpose : partitions information.
//

#ifndef ABSTRACT_PART_INFO_HEADER
#define ABSTRACT_PART_INFO_HEADER

#include "Support.h"

//
// class
//
class CAbstractPartInfo
{
public:
	virtual						~CAbstractPartInfo() {};

	/**
	 * Insert PartInfo
	 * @pParent : parent node
	 * @pInsertAfter : insert item after this item
	 * @dwFlags: flags
	 */
	virtual CAbstractPartInfo*	Insert(CAbstractPartInfo* pParent, CAbstractPartInfo* pInsertAfter, DWORD dwFlags) = 0;

	// iterate on items
	virtual CAbstractPartInfo*	GetParent() const = 0;
	virtual CAbstractPartInfo*	GetChild() const = 0;
	virtual CAbstractPartInfo*	GetNext() const = 0;

	// set items
	virtual void				SetParent(CAbstractPartInfo* pParent) = 0;
	virtual void				SetChild(CAbstractPartInfo* pChild) = 0;
	virtual void				SetNext(CAbstractPartInfo* pNext) = 0;

	// Get/Set flags
	virtual DWORD				GetFlags() const = 0;
	virtual BOOL				IsFlagExists(DWORD dwFlag) const = 0;
	virtual void				SetFlags(DWORD dwFlag, BOOL bAdd) = 0;

	// Get/Set number of sectors
	virtual INT64				GetSectors() const = 0;
	virtual void				SetSectors(INT64 i64Sectors) = 0;

	/**
	 * Get start sector
	 *
	 */
	virtual INT64				GetStartSector() const = 0;
	/**
	 * Set start sector
	 *
	 * @i64Sector : sector number
	 */
	virtual void				SetStartSector(INT64 i64Sector) = 0;

	// Get/Set Partition Type
	virtual BYTE				GetPartitionType() const = 0;
	virtual void				SetPartitionType(BYTE cType) = 0;

	/**
	 * Get partition type GUID
	 */
	virtual GUID				GetPartitionTypeGUID() const = 0;
	/**
	 * Set partition type GUID
	 */
	virtual void				SetPartitionTypeGUID(GUID Type) = 0;

	/**
	 * Get disk number
	 */
	virtual int					GetDiskNumber() const = 0;
	/**
	 * Set disk number
	 * param
	 * @nDiskNumber : disk number to set
	 */
	virtual void				SetDiskNumber(int nDiskNumber) = 0;

	/**
	 * Get partition number
	 */
	virtual int					GetPartitionNumber() const = 0;
	/**
	 * Set partition number
	 * param
	 * @nPartitionNumber : partition number to set
	 */
	virtual void				SetPartitionNumber(int nPartitionNumber) = 0;
};

//
// end of class
//

#endif // ABSTRACT_PART_INFO_HEADER

//
// End of file.
//
