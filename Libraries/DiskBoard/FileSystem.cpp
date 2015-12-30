/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading NT file system.
 */

#include "Support.h"
#include "AbstractLog.h"
#include "AbstractDisk.h"
#include "AbstractPartInfo.h"
#include "NTFS.h"
#include "Disk.h"
#include "MGAPI.h"
#include "CoreString.h"
#include "FileSystemNTFS.h"
#include "DiskBoardInterface.h"

#include "FileSystem.h"

CFileSystem::CFileSystem(CAbstractLog* pLog)
{
	m_pLog = pLog;
	m_pThisInterface = new CDiskBoardInterface();
	if (m_pThisInterface)
		m_pDisk = m_pThisInterface->CreateDisk(m_pLog);
}

CFileSystem::~CFileSystem()
{
	if (m_pThisInterface) {
		m_pThisInterface->DeleteDisk(m_pDisk);

		DELETEME(m_pThisInterface);
	}
}

/**
 * Initialize
 * Initialize file system
 * @pPartInfo: Partition to be scanned for meta data and data
 */
BOOL CFileSystem::Initialize(CAbstractPartInfo* pPartInfo)
{
	BOOL bRes = TRUE;
	CNTFSBootSector* pNTFS = NULL;
	size_t nSectorSize = 0;
	DWORD dwSectorsRead = 0;
	BYTE* pBuffer = NULL;

	try {
		if (!pPartInfo || !m_pDisk) {
			ASSERT(FALSE);
			throw _E_REP_PARAM_ERROR;
		}

		if (m_pDisk->OpenDisk(pPartInfo->GetDiskNumber(), GENERIC_READ) == -1) {
			CCoreString csMessage;
			csMessage.Format(_T("Could not open disk %d (0x%x)."),
				pPartInfo->GetDiskNumber(),
				pPartInfo->GetDiskNumber());
			m_pLog->AddLog(csMessage, __TFILE__, __LINE__);
			throw _E_REP_DRIVE_ERROR;
		}

		m_pDisk->GetSectorSize(&nSectorSize);
		pBuffer = new BYTE[nSectorSize];
		TEST_AND_THROW(!pBuffer, _E_REP_MEMORY_FAIL);
		ZeroMemory(pBuffer, nSectorSize);

		/*
		 * create analyzer class, which will read the boot sector of partition
		 * and set the partition type accordingly
		 */
		if (m_pDisk->ReadDisk(pBuffer, pPartInfo->GetStartSector(), 1, dwSectorsRead) != 0) {
			CCoreString csMessage;
			csMessage.Format(_T("Drive read failed at sector %d (0x%x)"),
				pPartInfo->GetStartSector(),
				pPartInfo->GetStartSector());
			m_pLog->AddLog(csMessage, __TFILE__, __LINE__);
			throw _E_REP_DRIVE_ERROR;
		}

		pNTFS = (CNTFSBootSector*) pBuffer;
		if (pNTFS && pNTFS->IsValid()) {
			m_pFileSystemNTFS = new CFileSystemNTFS();
			m_pFileSystemNTFS->SetMFTClusterNum(pNTFS->GetMFTClusterNum());
			m_pFileSystemNTFS->SetBytesPerSector(pNTFS->GetBytesPerSector());
			m_pFileSystemNTFS->SetSectorsPerCluster(pNTFS->GetSectorsPerCluster());
			m_pFileSystemNTFS->SetBytesPerCluster(pNTFS->GetSectorsPerCluster() * pNTFS->GetBytesPerSector());
			m_pFileSystemNTFS->SetClustersPerMFTRecord(pNTFS->GetClustersPerMFTRecord());
		}
		else {
			ASSERT(FALSE);
			m_pLog->AddLog(_T("Recovery for File system not implemented"), __TFILE__, __LINE__);
			throw _E_REP_NOT_IMPLEMENTED;
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

	return bRes;
}

/**
 * Scan
 * Read file system meta data, and data
 * This data will be shown to user, and to be used for recovery
 * @pPartInfo: Partition to be scanned for meta data and data
 */
BOOL CFileSystem::Scan()
{
	BOOL bRes = TRUE;

	try {
		if (m_pFileSystemNTFS) {
			bRes = m_pFileSystemNTFS->Scan();
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

	return bRes;
}

/**
 * Recover
 * Start recovering files
 */
BOOL CFileSystem::Recover()
{
	ASSERT(FALSE);
	return FALSE;
}

/**
 * Stop
 * Stop the scan/recovery process
 */
BOOL CFileSystem::Stop()
{
	ASSERT(FALSE);
	return FALSE;
}
