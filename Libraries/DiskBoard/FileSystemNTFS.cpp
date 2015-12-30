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
#include "MGAPI.h"
#include "FileSystemNTFS.h"

CFileSystemNTFS::CFileSystemNTFS() : CFileSystem(m_pLog)
{
}

CFileSystemNTFS::~CFileSystemNTFS()
{
}

/**
 * Scan
 * Read file system meta data, and data
 * This data will be shown to user, and to be used for recovery
 */
BOOL CFileSystemNTFS::Scan()
{
	BOOL bRes = TRUE;

	try {
		bRes = ReadMFTData();
	}
	catch (const int nException) {
		SetErrorNumber(nException);
		if (m_pLog)
			m_pLog->AddLog(_T("Exception caught..."), __TFILE__, __LINE__, GetErrorMessage(GetErrorNumber()));
		bRes = FALSE;
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
		if (m_pLog)
			m_pLog->AddLog(_T("Unexpected exception caught..."), __TFILE__, __LINE__);
		bRes = FALSE;
	}

	return bRes;
}

/**
 * Recover
 * Start recovering files
 */
BOOL CFileSystemNTFS::Recover()
{
	ASSERT(FALSE);
	return FALSE;
}

BOOL CFileSystemNTFS::Stop()
{
	ASSERT(FALSE);
	return FALSE;
}


void CFileSystemNTFS::SetMFTClusterNum(u64 u64MFT)
{
	m_u64MFT = u64MFT;
}

void CFileSystemNTFS::SetBytesPerSector(u16 u16BytesPerSector)
{
	m_u16BytesPerSector = u16BytesPerSector;
}

void CFileSystemNTFS::SetSectorsPerCluster(u8 u8SectorsPerCluster)
{
	m_u8SectorsPerCluster = u8SectorsPerCluster;
}

void CFileSystemNTFS::SetBytesPerCluster(u32 u32BytesPerCluster)
{
	m_u32BytesPerCluster = u32BytesPerCluster;
}

void CFileSystemNTFS::SetClustersPerMFTRecord(s8 s8ClusterPerMFTRecord)
{
	m_s8ClustersPerMFTRecord = s8ClusterPerMFTRecord;
}

BOOL CFileSystemNTFS::ReadMFTData()
{
	BOOL bRes = FALSE;
	try {

	}
	catch (const int nException) {
		SetErrorNumber(nException);
		if (m_pLog)
			m_pLog->AddLog(_T("Exception caught..."), __TFILE__, __LINE__, GetErrorMessage(GetErrorNumber()));
		bRes = FALSE;
	}
	catch (...) {
		SetErrorNumber(_E_REP_UNEXPECTED);
		if (m_pLog)
			m_pLog->AddLog(_T("Unexpected exception caught..."), __TFILE__, __LINE__);
		bRes = FALSE;
	}
	return bRes;
}