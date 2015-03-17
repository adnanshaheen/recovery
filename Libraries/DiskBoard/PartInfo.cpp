/**
 * Project : Disk Board
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Disk(s)/Partition(s) information tree
 */

#include "PartInfo.h"

CPartInfo::CPartInfo(CAbstractLog* pLog)
{
	m_pLog = pLog;
}

CPartInfo::~CPartInfo()
{
}

/**
 * Insert PartInfo
 */
CAbstractPartInfo* CPartInfo::Insert(CAbstractPartInfo* pParent, CAbstractPartInfo* pInsertAfter, DWORD dwFlags)
{
	if (!pParent) {
		ASSERT(FALSE);
		return NULL;
	}

	CAbstractPartInfo* pPartInfo = new CPartInfo(m_pLog);
	if (pPartInfo) {
		if (dwFlags & MG_PARTINFO_DISK) {
			pPartInfo->SetParent(pParent);
			pPartInfo->SetFlags(MG_PARTINFO_DISK, TRUE);
			if (pInsertAfter) {
				pPartInfo->SetNext(pInsertAfter->GetNext());
				pInsertAfter->SetNext(pPartInfo);
			}
			else {
				while (pParent->GetChild())
					pParent = pParent->GetChild();

				pParent->SetChild(pPartInfo);
			}
		}
		else {
		}
	}

	return pPartInfo;
}

// iterate on items
CAbstractPartInfo* CPartInfo::GetParent() const
{
	return NULL;
}
CAbstractPartInfo* CPartInfo::GetChild() const
{
	return NULL;
}
CAbstractPartInfo* CPartInfo::GetNext() const
{
	return NULL;
}
CAbstractPartInfo* CPartInfo::GetPrev() const
{
	return NULL;
}

// set items
void CPartInfo::SetParent(CAbstractPartInfo* pParent)
{
}
void CPartInfo::SetChild(CAbstractPartInfo* pChild)
{
}
void CPartInfo::SetNext(CAbstractPartInfo* pNext)
{
}

// Get/Set flags
DWORD CPartInfo::GetFlags() const
{
	return 0;
}
BOOL CPartInfo::IsFlagExists(DWORD dwFlag) const
{
	return FALSE;
}
void CPartInfo::SetFlags(DWORD dwFlag, BOOL bAdd)
{
}

// Get/Set number of sectors
INT64 CPartInfo::GetNumofSectors() const
{
	return 0;
}
void CPartInfo::SetNumofSectors(INT64 i64Sectors)
{
}

// Get/Set start sector
INT64 CPartInfo::GetStartSector() const
{
	return 0;
}
void CPartInfo::SetStartSector(INT64 i64Sector)
{
}

// Get/Set Partition Type
BYTE CPartInfo::GetPartitionType() const
{
	return 0;
}
void CPartInfo::SetPartitionType(BYTE cType)
{
}
