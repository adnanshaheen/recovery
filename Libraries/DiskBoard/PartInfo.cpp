/**
 * Project : Disk Board
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Disk(s)/Partition(s) information tree
 */

#include "PartInfo.h"

class CPartInfoData
{
public:
	CPartInfoData()
	{
		memset(this, 0, sizeof(*this));
		m_nDiskNumber = -1;			// Disk number
		m_nPartitionNumber = -1;	// Partition number
	}

public:
	CAbstractPartInfo*	m_pParent;
	CAbstractPartInfo*	m_pNext;
	CAbstractPartInfo*	m_pChild;

	int					m_nDiskNumber;				// Disk number
	int					m_nPartitionNumber;			// Partition number

	INT64				m_i64Sectors;				// Number of sectors
	INT64				m_i64StartSector;			// Start sector of this item
	INT64				m_i64PTableStartSector;		// Start sector of partition table.

	BYTE				m_PartitionType;			// File system used on the partition
	GUID				m_GuidPartitionType;		// File system used on the guid partition
	DWORD				m_dwInfoFlags;
};

CPartInfo::CPartInfo(CAbstractLog* pLog)
{
	m_pLog = pLog;
	m_pPartInfoData = new CPartInfoData();
}

CPartInfo::~CPartInfo()
{
	DELETEME(m_pPartInfoData);
}

/**
 * Insert PartInfo
 * @pParent : parent node
 * @pInsertAfter : insert item after this item
 * @dwFlags: flags
 */
CAbstractPartInfo* CPartInfo::Insert(CAbstractPartInfo* pParent, CAbstractPartInfo* pInsertAfter, DWORD dwFlags)
{
	if (!pParent) {
		ASSERT(FALSE);
		return nullptr;
	}

	CAbstractPartInfo* pPartInfo = new CPartInfo(m_pLog);
	if (pPartInfo) {

		pPartInfo->SetFlags(dwFlags, TRUE);
		pPartInfo->SetParent(pParent);
		if (pInsertAfter)
		{
			pPartInfo->SetNext(pInsertAfter->GetNext());
			pInsertAfter->SetNext(pPartInfo);
		}
		else
		{
			if (pParent->GetChild() != 0)
				pPartInfo->SetNext(pParent->GetChild());
			pParent->SetChild(pPartInfo);
		}
	}

	return pPartInfo;
}

// iterate on items
CAbstractPartInfo* CPartInfo::GetParent() const
{
	return m_pPartInfoData->m_pParent;
}
CAbstractPartInfo* CPartInfo::GetChild() const
{
	return m_pPartInfoData->m_pChild;
}
CAbstractPartInfo* CPartInfo::GetNext() const
{
	return m_pPartInfoData->m_pNext;
}

// set items
void CPartInfo::SetParent(CAbstractPartInfo* pParent)
{
	m_pPartInfoData->m_pParent = pParent;
}
void CPartInfo::SetChild(CAbstractPartInfo* pChild)
{
	m_pPartInfoData->m_pChild = pChild;
}
void CPartInfo::SetNext(CAbstractPartInfo* pNext)
{
	m_pPartInfoData->m_pNext = pNext;
}

// Get/Set flags
DWORD CPartInfo::GetFlags() const
{
	return m_pPartInfoData->m_dwInfoFlags;
}
BOOL CPartInfo::IsFlagExists(DWORD dwFlag) const
{
	return m_pPartInfoData->m_dwInfoFlags & dwFlag;
}
void CPartInfo::SetFlags(DWORD dwFlag, BOOL bAdd)
{
	if (bAdd)
		m_pPartInfoData->m_dwInfoFlags |= dwFlag;
	else
		m_pPartInfoData->m_dwInfoFlags &= ~dwFlag;
}

// Get/Set number of sectors
INT64 CPartInfo::GetSectors() const
{
	return m_pPartInfoData->m_i64Sectors;
}
void CPartInfo::SetSectors(INT64 i64Sectors)
{
	m_pPartInfoData->m_i64Sectors = i64Sectors;
}

/**
 * Get start sector
 */
INT64 CPartInfo::GetStartSector() const
{
	return m_pPartInfoData->m_i64StartSector;
}
/**
 * Set start sector
 *
 * @i64Sector : sector number
 */
void CPartInfo::SetStartSector(INT64 i64Sector)
{
	m_pPartInfoData->m_i64StartSector = i64Sector;
}

// Get/Set Partition Type
BYTE CPartInfo::GetPartitionType() const
{
	return m_pPartInfoData->m_PartitionType;
}
void CPartInfo::SetPartitionType(BYTE cType)
{
	m_pPartInfoData->m_PartitionType = cType;
}

/**
 * Get partition type GUID
 */
GUID CPartInfo::GetPartitionTypeGUID() const
{
	return m_pPartInfoData->m_GuidPartitionType;
}
/**
 * Set partition type GUID
 */
void CPartInfo::SetPartitionTypeGUID(GUID Type)
{
	m_pPartInfoData->m_GuidPartitionType = Type;
}

/**
 * Get disk number
 */
int CPartInfo::GetDiskNumber() const
{
	return m_pPartInfoData->m_nDiskNumber;
}
/**
 * Set disk number
 * param
 * @nDiskNumber : disk number to set
 */
void CPartInfo::SetDiskNumber(int nDiskNumber)
{
	m_pPartInfoData->m_nDiskNumber = nDiskNumber;
}

/**
 * Get partition number
 */
int CPartInfo::GetPartitionNumber() const
{
	return m_pPartInfoData->m_nPartitionNumber;
}
/**
 * Set partition number
 * param
 * @nPartitionNumber : partition number to set
 */
void CPartInfo::SetPartitionNumber(int nPartitionNumber)
{
	m_pPartInfoData->m_nPartitionNumber = nPartitionNumber;
}

