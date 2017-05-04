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
#include "NTFS.h"
#include "Fat.h"

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
	CAbstractDisk* pDisk = nullptr;
	CAbstractPartInfo* pDiskInfo = nullptr;
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
				pDiskInfo->SetDiskNumber(nDrive);

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

CAbstractPartInfo* CPartitioner::GetDiskItem() const
{
	return m_pPartInfo->GetChild();
}

void CPartitioner::AddHardDiskInfo(CAbstractDisk* pDisk, CAbstractPartInfo** pDiskInfo)
{
	CAbstractPartInfo* pInfo = nullptr;
	if (m_pPartInfo) {
		pInfo = m_pPartInfo->Insert(m_pPartInfo, *pDiskInfo, MG_PARTINFO_DISK);
		if (pInfo)
			*pDiskInfo = pInfo;
	}
}

/**
 * Read partition table, and add info items in tree
 *
 * @pDisk: read from disk
 * @pDiskInfo: disk item info (parent item of partitions)
 */
BOOL CPartitioner::ReadPartitions(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo)
{
	BOOL bRes = TRUE;
	size_t nSectorSize = 0;
	DWORD dwSectorsRead = 0;
	BYTE* pBuffer = nullptr;
	CRootSector cPartitionTable;
	CAbstractPartInfo* pInfo = nullptr;

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

							int nDiskNumber = -1;
							pDisk->GetDiskNumber(&nDiskNumber);
							pInfo->SetDiskNumber(nDiskNumber);
							pInfo->SetPartitionType(cPartitionTable[nPart].PartitionType());
							pInfo->SetSectors(cPartitionTable[nPart].NumSectors());
							//pInfo->SetStartSector(cPartitionTable[nPart].Start());

							if (pInfo->IsFlagExists(MG_PARTINFO_GUID)) {

								/**
								 * This is GUID disk
								 * Read the partitions
								 */
								bRes = ReadGPTPartitions(pDisk, pDiskInfo, pInfo);
								if (!bRes)
									throw _E_REP_GUID_PART_FAIL;

								pDiskInfo->SetFlags(MG_PARTINFO_GUID, TRUE);
								//bRes = ReadGPTPartType(pDisk, pDiskInfo);
							}
						}
					}
				}
			}
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
		bRes = FALSE;
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
		bRes = FALSE;
	}

	if (pBuffer)
		DELETE_ARRAY(pBuffer);

	return bRes;
}

/**
 * Read GUID partition table, and add info items in tree
 *
 * @pDisk: read from disk
 * @pDiskInfo: disk item info (parent item of partitions)
 * @pInsertAfter: insert new partition items after this item
 */
BOOL CPartitioner::ReadGPTPartitions(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo, CAbstractPartInfo* pInsertAfter)
{
	BOOL bRes = TRUE;
	size_t nSectorSize = 0;
	DWORD dwSectorsRead = 0;
	s32 nIndex = 2;
	BYTE* pBuffer = nullptr;
	BYTE* pPartEntry = nullptr;
	CGPTHeader cPartitionTableHeader;
	CGPTPartitions cGPTPartition;
	CAbstractPartInfo* pInfo = nullptr;

	try {
		if (!pDisk || ! pDiskInfo) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		pDisk->GetSectorSize(&nSectorSize);
		pBuffer = new BYTE[nSectorSize];
		ZeroMemory(pBuffer, nSectorSize);

		if (pDisk->ReadDisk(pBuffer, 1, 1, dwSectorsRead) != 0) {
			m_pLog->AddLog(_T("Drive read failed at sector 1"), __TFILE__, __LINE__);
			throw _E_REP_DRIVE_ERROR;
		}

		cPartitionTableHeader = *(CGPTHeader*) pBuffer;
		if (cPartitionTableHeader.IsValid()) {

			/*
			 * We have a valid "EFI PART"
			 * start reading partition entries
			 */
			pPartEntry = new BYTE[nSectorSize];
			int nDiskNumber = -1;

			for (uint32_t nPart = 0; nPart < cPartitionTableHeader.GetPartitions(); ++ nPart) {

				ZeroMemory(pPartEntry, nSectorSize);

				if (pDisk->ReadDisk(pPartEntry, nIndex ++, 1, dwSectorsRead) != 0) {

					CCoreString csMessage;
					csMessage.Format(_T("Drive read failed at sector " FMTlld), ARGlld(cPartitionTableHeader.GetFirstEntry()));
					m_pLog->AddLog(csMessage, __TFILE__, __LINE__);
					throw _E_REP_DRIVE_ERROR;
				}

				pInfo = AddPartition(pDiskInfo, pInsertAfter, pPartEntry);
				if (pInfo) {
					nDiskNumber = -1;
					pDisk->GetDiskNumber(&nDiskNumber);
					pInfo->SetDiskNumber(nDiskNumber);
					pInsertAfter = pInfo;
				}
				pInfo = AddPartition(pDiskInfo, pInsertAfter, pPartEntry + cPartitionTableHeader.GetSize());
				if (pInfo) {
					pInfo->SetDiskNumber(nDiskNumber);
					pInsertAfter = pInfo;
				}

				pInfo = AddPartition(pDiskInfo, pInsertAfter, pPartEntry + cPartitionTableHeader.GetSize() * 2);
				if (pInfo) {
					pInfo->SetDiskNumber(nDiskNumber);
					pInsertAfter = pInfo;
				}

				pInfo = AddPartition(pDiskInfo, pInsertAfter, pPartEntry + cPartitionTableHeader.GetSize() * 3);
				if (pInfo) {
					pInfo->SetDiskNumber(nDiskNumber);
					pInsertAfter = pInfo;
				}
			}
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
		bRes = FALSE;
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
		bRes = FALSE;
	}

	if (pPartEntry)
		DELETE_ARRAY(pPartEntry);
	if (pBuffer)
		DELETE_ARRAY(pBuffer);

	return bRes;
}

CAbstractPartInfo* CPartitioner::AddPartition(CAbstractPartInfo* pDiskInfo, CAbstractPartInfo* pInsertAfter, void* pPartBuffer)
{
	CAbstractPartInfo* pInfo = nullptr;
	CGPTPartitions* pGPTPartition = (CGPTPartitions*) pPartBuffer;

	try {
		if (!pGPTPartition || !pDiskInfo) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		if (pGPTPartition->IsValid()) {
			if (m_pPartInfo) {

				pInfo = m_pPartInfo->Insert(
					pDiskInfo,
					pInsertAfter,
					MG_PARTINFO_GUID);
				if (pInfo) {

					pInfo->SetPartitionTypeGUID(pGPTPartition->PartitionType());
					pInfo->SetSectors(pGPTPartition->NumSectors());
					pInfo->SetStartSector(pGPTPartition->GetStartSector());
					if (pGPTPartition->IsDataDisk())
						pInfo->SetFlags(MG_PARTINFO_DATA, TRUE);
				}
			}
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
	}

	return pInfo;
}


BOOL CPartitioner::ReadGPTPartType(CAbstractDisk* pDisk, CAbstractPartInfo* pDiskInfo)
{
	BOOL bRes = TRUE;
	CFat16* pFat16 = nullptr;
	CFat32* pFat32 = nullptr;
	CExFat* pExFat = nullptr;
	CNTFSBootSector* pNTFS = nullptr;
	size_t nSectorSize = 0;
	DWORD dwSectorsRead = 0;
	BYTE* pBuffer = nullptr;

	try {
		if (!pDisk || !pDiskInfo) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		pDisk->GetSectorSize(&nSectorSize);
		pBuffer = new BYTE[nSectorSize];
		TEST_AND_THROW(!pBuffer, _E_REP_MEMORY_FAIL);
		ZeroMemory(pBuffer, nSectorSize);

		CAbstractPartInfo* pPartInfo = pDiskInfo->GetChild();
		while (pPartInfo) {

			/* currently only checking for windows data partitions */
			if (pPartInfo->IsFlagExists(MG_PARTINFO_DATA)) {

				/*
				 * create analyzer class, which will read the boot sector of partition
				 * and set the partition type accordingly
				 */
				if (pDisk->ReadDisk(pBuffer, pPartInfo->GetStartSector(), 1, dwSectorsRead) != 0) {
					m_pLog->AddLog(_T("Drive read failed at sector 0"), __TFILE__, __LINE__);
					throw _E_REP_DRIVE_ERROR;
				}

				pFat16 = (CFat16*) pBuffer;
				pFat32 = (CFat32*) pBuffer;
				pExFat = (CExFat*) pBuffer;
				pNTFS = (CNTFSBootSector*) pBuffer;
			}

			pPartInfo = pPartInfo->GetNext();
		}
	}
	catch (int nException) {
		SetErrorNumber(nException);
		bRes = FALSE;
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
		bRes = FALSE;
	}

	if (pBuffer)
		DELETE_ARRAY(pBuffer);

	return bRes;
}
