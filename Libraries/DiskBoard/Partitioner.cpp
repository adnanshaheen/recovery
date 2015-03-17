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
#include "Partitioner.h"
#include "MGAPI.h"

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

	try {
		pDisk = m_pDiskBoard->CreateDisk(m_pLog);
		TEST_AND_THROW(pDisk, _E_REP_MEMORY_FAIL);

		for (int nDrive = 0; nDrive < MAX_HDD_SUPPORTED; ++ nDrive) {

			if (pDisk->OpenDisk(nDrive, GENERIC_READ) == -1) {

				/* set last app error, and continue to next disk */
				SetErrorNumber(_E_REP_DRIVE_ERROR);
				continue;
			}

			if (pDisk->IsDiskExists()) {

				/* add disk info, as we have found one */
				AddHardDiskInfo(pDisk, &pDiskInfo);
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
