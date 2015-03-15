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

	// iterate on items
	virtual CAbstractPartInfo*	GetParent() const = 0;
	virtual CAbstractPartInfo*	GetChild() const = 0;
	virtual CAbstractPartInfo*	GetNext() const = 0;
	virtual CAbstractPartInfo*	GetPrev() const = 0;

	// set items
	virtual void				SetParent(CAbstractPartInfo* pParent) = 0;
	virtual void				SetChild(CAbstractPartInfo* pChild) = 0;
	virtual void				SetNext(CAbstractPartInfo* pNext) = 0;

	// Get/Set flags
	virtual DWORD				GetFlags() const = 0;
	virtual BOOL				IsFlagExists(DWORD dwFlag) const = 0;
	virtual void				SetFlags(DWORD dwFlag, BOOL bAdd) = 0;

	// Get/Set number of sectors
	virtual INT64				GetNumofSectors() const = 0;
	virtual void				SetNumofSectors(INT64 i64Sectors) = 0;

	// Get/Set start sector
	virtual INT64				GetStartSector() const = 0;
	virtual void				SetStartSector(INT64 i64Sector) = 0;

	// Get/Set Partition Type
	virtual BYTE				GetPartitionType() const = 0;
	virtual void				SetPartitionType(BYTE cType) = 0;

};

//
// end of class
//

#endif // ABSTRACT_PART_INFO_HEADER

//
// End of file.
//
