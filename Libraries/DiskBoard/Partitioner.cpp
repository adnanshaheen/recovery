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
	CAbstractPartInfo* pInfo = NULL;
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

							int nDiskNumber = -1;
							pDisk->GetDiskNumber(&nDiskNumber);
							pInfo->SetDiskNumber(nDiskNumber);
							pInfo->SetPartitionType(cPartitionTable[nPart].PartitionType());
							pInfo->SetSectors(cPartitionTable[nPart].NumSectors());

							if (pInfo->IsFlagExists(MG_PARTINFO_GUID)) {

								/**
								 * This is GUID disk
								 * Read the partitions
								 */
								ReadGPTPartitions(pDisk, pDiskInfo, pInfo);
							}
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
	BYTE* pBuffer = NULL;
	CGPTHeader cPartitionTableHeader;
	CAbstractPartInfo* pInfo = NULL;

	try {
		if (!pDisk || ! pDiskInfo) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		pDisk->GetSectorSize(&nSectorSize);
		pBuffer = new BYTE[nSectorSize];
		ZeroMemory(pBuffer, nSectorSize);

		if (pDisk->ReadDisk(pBuffer, 1, 1, dwSectorsRead) != 0) {
			m_pLog->AddLog(_T("Drive read failed at sector 0"), __TFILE__, __LINE__);
			throw _E_REP_DRIVE_ERROR;
		}

		cPartitionTableHeader = *(CGPTHeader*) pBuffer;
		if (cPartitionTableHeader.IsValid()) {

			PrintGUIDHeader(&cPartitionTableHeader);
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

#ifdef DEBUG
void CPartitioner::PrintGUIDHeader(void* pBuffer) const
{
	CGPTHeader* pHeader = (CGPTHeader*) pBuffer;
	if (pHeader) {

		m_pLog->AddLog(_T("*** GUID PARTITION TABLE HEADER ***"));

		CCoreString cMessage;
		cMessage.Format(FMTlld, ARGlld(pHeader->m_cMagic));
		m_pLog->AddLog(_T("Magic: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nRevision));
		m_pLog->AddLog(_T("Revision: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nHeaderSize));
		m_pLog->AddLog(_T("Header Size: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nCRC32_Header));
		m_pLog->AddLog(_T("Header CRC: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_i64LBA));
		m_pLog->AddLog(_T("Current LBA: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTlld, ARGlld(pHeader->m_i64BackupLBA));
		m_pLog->AddLog(_T("Backup LBA: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTlld, ARGlld(pHeader->m_i64FirstLBA));
		m_pLog->AddLog(_T("First LBA Partition: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTlld, ARGlld(pHeader->m_i64LastLBA));
		m_pLog->AddLog(_T("Last LBA Partition: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTlc, ARGlc(pHeader->m_DiskGUID));
		m_pLog->AddLog(_T("Disk GUID: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTlld, ARGlld(pHeader->m_i64StartingLBA));
		m_pLog->AddLog(_T("Partition entry LBA: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nNumber));
		m_pLog->AddLog(_T("Partition entries: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nSize));
		m_pLog->AddLog(_T("Size of partition entry: "));
		m_pLog->AddLog(cMessage);

		cMessage.Format(FMTd, ARGd(pHeader->m_nCRC32_Table));
		m_pLog->AddLog(_T("Partition array CRC: "));
		m_pLog->AddLog(cMessage);
	}
}
#endif /* DEBUG */
