//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Disk access.
//

//
// headers
//
#include "Types.h"
#include "Disk.h"
#include "AbstractLog.h"
#include "AbstractFile.h"
#include "CoreString.h"
//
// constructor
//
CDisk::CDisk()
{
	m_pLog = NULL;
	m_pFile = NULL;
}
CDisk::CDisk(CAbstractLog* pLog)
{
	m_pLog = pLog;
	m_pFile = NULL;
}

//
// destructor
//
/*virtual */CDisk::~CDisk()
{
}

//
// Open a disk with disk number.
//
/*virtual */int CDisk::OpenDisk(size_t nDisk, DWORD dwFlags)
{
	if (nDisk > MAX_DRIVE_SUPPORT)
	{
		// we are not supporting such a big number.
#ifdef DEBUG
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("More than %d hard drives are not supported!!!"), __TFILE__, __LINE__, MAX_DRIVE_SUPPORT);
#endif // DEBUG
		return -1;
	}

	CCoreString csDisk;
	csDisk.Format(_T("PHYSICALDRIVE%d"), nDisk);
	return OpenDisk(csDisk, dwFlags);
}

//
// Open a disk with name.
//
/*virtual */int CDisk::OpenDisk(LPCTSTR pszDisk, DWORD dwFlags)
{
	// Parameter check.
	if (pszDisk == NULL || _tcslen(pszDisk) == 0)
	{
		ASSERT(FALSE);
		return -1;
	}

	// Close already open disk.
	if (m_pFile != NULL && m_pFile->IsFileOpen())
		m_pFile->CloseFile();

	// Try to open the disk.
	int iRes = m_pFile->CreateFile(pszDisk,
								dwFlags,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								OPEN_EXISTING,
								FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_NO_BUFFERING);

	// Error check.
	if (iRes < 0)
	{
		// Error already logged at CAbstractFile
		return -1;
	}

	return 0;
}

//
// Open the disk with offset.
//
/*virtual */int CDisk::OpenDisk(UINT64 ui64Offset, UINT64 ui64Length, size_t nDisk, DWORD dwFlags)
{
	int nRes = 0;
	nRes = OpenDisk(nDisk, dwFlags);
	if (nRes == 0)
	{
		m_ui64StartSector = ui64Offset;
		m_ui64DiskSize = ui64Length;

		INT64 i64NewPos = 0;
		if (SetDiskPointer(0, i64NewPos) < 0)
		{
			ASSERT(FALSE);
			return -1;
		}

		nRes = i64NewPos == 0 ? nRes : -1;
	}

	return nRes;
}

//
// Close the disk.
//
/*virtual */int CDisk::CloseDisk()
{
	if (m_pFile == NULL)
		return 0;

	if (m_pFile->IsFileOpen())
	{
		if (m_pFile->CloseFile())
			return 0;
		else
			return -1;
	}

	return 0;
}

//
// Read the buffer from disk.
//
/*virtual */int CDisk::ReadDisk(BYTE* pBuffer, DWORD dwSectorsToRead, DWORD& dwSectorsRead)
{
	if (m_pFile == NULL || !m_pFile->IsFileOpen())
	{
		ASSERT(FALSE);
		return -1;
	}

	INT64 i64DiskPos = 0;
	if (GetDiskPointer(i64DiskPos) < 0)
	{
		ASSERT(FALSE);
		return -1;
	}

	if ((m_ui64DiskSize == 0) || ((i64DiskPos + dwSectorsRead) > m_ui64DiskSize))
	{
#ifdef DEBUG
		if (m_pLog != NULL)
			m_pLog->AddLog(_T("Invalid boundry read detected!!!"), __TFILE__, __LINE__);
#endif // DEBUG
		return -1;
	}

	int nRes = m_pFile->ReadFile(pBuffer, dwSectorsToRead * m_cDiskGeometry.BytesPerSector, &dwSectorsRead);
	dwSectorsRead /= m_cDiskGeometry.BytesPerSector;

	return nRes;
}

//
// Read the buffer on specified positition from disk.
//
/*virtual */int CDisk::ReadDisk(BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToRead, DWORD& dwSectorsRead)
{
	INT64 i64NewPos = 0;
	if (SetDiskPointer(ui64Sector, i64NewPos) < 0)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (ui64Sector != i64NewPos)
	{
		ASSERT(FALSE);
		return -1;
	}

	return ReadDisk(pBuffer, dwSectorsToRead, dwSectorsRead);
}

//
// Write the buffer to disk on current positition.
//
/*virtual */int CDisk::WriteDisk(const BYTE* pBuffer, DWORD dwSectorsToWrite, DWORD& dwSectorsWritten)
{
	return -1;
}

//
// Write the buffer to specified positition.
//
/*virtual */int CDisk::WriteDisk(const BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToWritten, DWORD& dwSectorsWritten)
{
	return -1;
}

//
// Get disk position.
//
/*virtual */int CDisk::GetDiskPointer(INT64& i64DiskPos)
{
	if (m_pFile == NULL || !m_pFile->IsFileOpen())
	{
		ASSERT(FALSE);
		return -1;
	}

	return m_pFile->GetFilePointer(i64DiskPos) ? 0 : -1;
}

//
// Set disk positition.
//
/*virtual */int CDisk::SetDiskPointer(INT64 i64DiskPos, INT64& i64NewPos)
{
	if (m_pFile == NULL || !m_pFile->IsFileOpen())
	{
		ASSERT(FALSE);
		return -1;
	}

	// Set the new sector number from the start of drive.
	i64DiskPos += m_ui64StartSector;
	BOOL bRes = m_pFile->SetFilePointer(i64DiskPos, i64NewPos);

	i64NewPos /= m_cDiskGeometry.BytesPerSector;
	i64NewPos -= m_ui64StartSector;

	return bRes ? 0 : -1;
}

//
// Flush the buffer
//
/*virtual */int CDisk::FlushDisk()
{
	return -1;
}

//
// Lock disk.
//
/*virtual */int CDisk::LockDisk()
{
	return -1;
}
/*virtual */int CDisk::LockDisk(size_t nDisk)
{
	return -1;
}

//
// Unlock the disk.
//
/*virtual */int CDisk::UnlockDisk()
{
	return -1;
}
/*virtual */int CDisk::UnlockDisk(size_t nDisk)
{
	return -1;
}

//
// Check if disk is locked.
//
/*virtual */BOOL CDisk::IsDiskLock() const
{
	return FALSE;
}

//
// Check for disk existance.
//
/*virtual */BOOL CDisk::IsDiskExists() const
{
	return FALSE;
}

//
// Check for disk with 8G limit.
//
/*virtual */BOOL CDisk::Is8GDisk() const
{
	return FALSE;
}

//
// Get start sector of disk.
//
/*virtual */BOOL CDisk::GetStartSector(INT64* p64StartSector) const
{
	return FALSE;
}

//
// Get total sectors.
//
/*virtual */BOOL CDisk::GetTotalSectors(INT64* p64Sectors) const
{
	return FALSE;
}

//
// Get sector size.
//
/*virtual */BOOL CDisk::GetSectorSize(size_t* pBytesPerSector) const
{
	return FALSE;
}

//
// Get disk number.
//
/*virtual */BOOL CDisk::GetDiskNumber(int* pDiskNumber) const
{
	return FALSE;
}

//
// Get disk geometry.
//
/*virtual */BOOL CDisk::GetDiskGeometry(INT64* p64Cylinders, size_t* pHeads, INT64* p64Sectors) const
{
	return FALSE;
}
/*virtual */BOOL CDisk::GetDiskGeometry(DISK_GEOMETRY& cDiskGeometry) const
{
	return FALSE;
}

//
// Get/Set sectors after geometry.
//
/*virtual */BOOL CDisk::GetSectorsAfterGeometry(size_t& nSectors) const
{
	return FALSE;
}
/*virtual */BOOL CDisk::SetSectorsAfterGeometry(size_t nSectors)
{
	return FALSE;
}

//
// Get disk model, vendor, and path.
//
/*virtual */BOOL CDisk::GetDiskInfo(size_t nDisk, TCHAR* pModel, size_t nModelBufferLength,
							TCHAR* pVendor, size_t nVendorBufferLength,
							TCHAR* pPath, size_t nPathBufferLength) const
{
	return FALSE;
}

//
// end of file
//
