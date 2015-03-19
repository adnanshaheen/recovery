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
		m_pParent = NULL;
		m_pNext = NULL;
		m_pChild = NULL;
		m_nDiskNumber = -1;			// Disk number
		m_nPartitionNumber = -1;	// Partition number

		m_i64Sectors = 0;			// Number of sectors
		m_i64StartSector = 0;		// Start sector of this item
		m_i64PTableStartSector = 0;	// Start sector of partition table.

		m_PartitionType = 0;		// File system used on the partition
		m_dwInfoFlags = 0;
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
		return NULL;
	}

	CAbstractPartInfo* pPartInfo = new CPartInfo(m_pLog);
	if (pPartInfo) {

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
	m_pPartInfoData->m_dwInfoFlags = (m_pPartInfoData->m_dwInfoFlags & ~dwFlag) || (bAdd ? dwFlag : 0);
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

// Get/Set start sector
INT64 CPartInfo::GetStartSector() const
{
	return m_pPartInfoData->m_i64StartSector;
}
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
