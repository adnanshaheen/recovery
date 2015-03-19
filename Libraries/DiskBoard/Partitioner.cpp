/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for partitions.
 */

#include "AbstractPartInfo.h"
#include "AbstractDiskBoardInterface.h"
#include "AbstractDisk.h"
#include "AbstractLog.h"
#include "Partitioner.h"
#include "MGAPI.h"
#include "PartitionTable.h"

CPartitioner::CPartitioner(const CAbstractDiskBoardInterface* pDiskBoard, CAbstractLog* pLog)
{
	m_pDiskBoard = pDiskBoard;
	m_pLog = pLog;
	m_pPartInfo = m_pDiskBoard->CreatePartInfo(m_pLog);
}

CPartitioner::~CPartitioner()
{
}

BOOL CPartitioner::Initialize()
{
	BOOL bRes = FALSE;
	CAbstractDisk* pDisk = NULL;
	CAbstractPartInfo* pDiskInfo = NULL;
	INT64 i64Sectors = 0;

	try {
		pDisk = m_pDiskBoard->CreateDisk(m_pLog);
		TEST_AND_THROW(!pDisk, _E_REP_MEMORY_FAIL);

		for (int nDrive = 0; nDrive < MAX_HDD_SUPPORTED; ++ nDrive) {

			if (pDisk->OpenDisk(nDrive, GENERIC_READ) == -1) {

				/* set last app error, and continue to next disk */
				SetErrorNumber(_E_REP_DRIVE_ERROR);
				continue;
			}

			if (pDisk->IsDiskExists()) {

				/* add disk info, as we have found one */
				AddHardDiskInfo(pDisk, &pDiskInfo);
				pDisk->GetTotalSectors(&i64Sectors);
				pDiskInfo->SetSectors(i64Sectors);

				ReadPartitions(pDisk, pDiskInfo);
				pDisk->CloseDisk();
			}
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
	}
	catch (...) {
	}

	if (pDisk)
		DELETEME(pDisk);

	return bRes;
}

void CPartitioner::AddHardDiskInfo(CAbstractDisk* pDisk, CAbstractPartInfo** pDiskInfo)
{
	CAbstractPartInfo* pInfo = NULL;
	if (m_pPartInfo) {
		pInfo = m_pPartInfo->Insert(m_pPartInfo, *pDiskInfo, MG_PARTINFO_DISK);
		if (pInfo)
			*pDiskInfo = pInfo;
	}
}

BOOL CPartitioner::ReadPartitions(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo)
{
	BOOL bRes = TRUE;
	size_t nSectorSize = 0;
	DWORD dwSectorsRead = 0;
	BYTE* pBuffer = NULL;
	CRootSector cPartitionTable;
	CAbstractPartInfo* pInfo = NULL;

	try {
		if (!pDisk || ! pDiskInfo) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		pDisk->GetSectorSize(&nSectorSize);
		pBuffer = new BYTE[nSectorSize];
		ZeroMemory(pBuffer, nSectorSize);

		if (pDisk->ReadDisk(pBuffer, 0, 1, dwSectorsRead) != 0) {
			m_pLog->AddLog(_T("Drive read failed at sector 0"), __TFILE__, __LINE__);
			throw _E_REP_DRIVE_ERROR;
		}

		cPartitionTable = *(CRootSector*) pBuffer;
		if (cPartitionTable.IsValid()) {

			for (int nPart = 0; nPart < 4; ++ nPart) {

				if (cPartitionTable[nPart].IsValid()) {

					if (m_pPartInfo) {
						pInfo = m_pPartInfo->Insert(
							pDiskInfo,
							pInfo,
							cPartitionTable[nPart].IsGPT() ? MG_PARTINFO_GUID : 0);
						if (pInfo) {

							pInfo->SetPartitionType(cPartitionTable[nPart].PartitionType());
							pInfo->SetSectors(cPartitionTable[nPart].NumSectors());
						}
					}
				}
			}
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
	}
	catch (...) {
	}

	if (pBuffer)
		DELETE_ARRAY(pBuffer);

	return bRes;
}
