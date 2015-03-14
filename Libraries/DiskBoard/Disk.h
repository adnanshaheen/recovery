//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Access hard disk.
//

#ifndef DISK_HEADER
#define DISK_HEADER

//
// decls
//
class CAbstractLog;
class CAbstractFile;

//
// headers
//
#include "AbstractDisk.h"

//
// class
//
class CDisk : public CAbstractDisk
{
public:
	CDisk();
	CDisk(CAbstractLog* pLog);
	virtual ~CDisk();

	// Open a disk with disk number.
	virtual int		OpenDisk(size_t nDisk, DWORD dwFlags);

	// Open a disk with name.
	virtual int		OpenDisk(LPCTSTR pszDisk, DWORD dwFlags);

	// Open the disk with offset.
	virtual int		OpenDisk(UINT64 ui64Offset, UINT64 ui64Length, size_t nDisk, DWORD dwFlags);

	// Close the disk.
	virtual int		CloseDisk();

	// Read the buffer from disk.
	virtual int		ReadDisk(BYTE* pBuffer, DWORD dwSectorsToRead, DWORD& dwSectorsRead);

	// Read the buffer on specified positition from disk.
	virtual int		ReadDisk(BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToRead, DWORD& dwSectorsRead);

	// Write the buffer to disk on current positition.
	virtual int		WriteDisk(const BYTE* pBuffer, DWORD dwSectorsToWrite, DWORD& dwSectorsWritten);

	// Write the buffer to specified positition.
	virtual int		WriteDisk(const BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToWritten, DWORD& dwSectorsWritten);

	// Get disk position.
	virtual int		GetDiskPointer(INT64& i64DiskPos);

	// Set disk positition.
	virtual int		SetDiskPointer(INT64 i64DiskPos, INT64& i64NewPos);

	// Flush the buffer.
	virtual int		FlushDisk();

	// Lock disk.
	virtual int		LockDisk();
	virtual int		LockDisk(size_t nDisk);

	// Unlock the disk.
	virtual int		UnlockDisk();
	virtual int		UnlockDisk(size_t nDisk);

	// Check if disk is locked.
	virtual BOOL	IsDiskLock() const;

	// Check for disk existance.
	virtual BOOL	IsDiskExists() const;

	// Check for disk with 8G limit.
	virtual BOOL	Is8GDisk() const;

	// Get start sector of disk.
	virtual BOOL	GetStartSector(INT64* p64StartSector) const;

	// Get total sectors.
	virtual BOOL	GetTotalSectors(INT64* p64Sectors) const;

	// Get sector size.
	virtual BOOL	GetSectorSize(size_t* pBytesPerSector) const;

	// Get disk number.
	virtual BOOL	GetDiskNumber(int* pDiskNumber) const;

	// Get disk geometry.
	virtual BOOL	GetDiskGeometry(INT64* p64Cylinders, size_t* pHeads, INT64* p64Sectors) const;
	virtual BOOL	GetDiskGeometry(DISK_GEOMETRY& cDiskGeometry) const;

	// Get/Set sectors after geometry.
	virtual BOOL	GetSectorsAfterGeometry(size_t& nSectors) const;
	virtual BOOL	SetSectorsAfterGeometry(size_t nSectors);

	// Get disk model, vendor, and path.
	virtual BOOL	GetDiskInfo(size_t nDisk, TCHAR* pModel, size_t nModelBufferLength,
								TCHAR* pVendor, size_t nVendorBufferLength,
								TCHAR* pPath, size_t nPathBufferLength) const;

private:
	UINT64			m_ui64StartSector;
	UINT64			m_ui64DiskSize;
	DISK_GEOMETRY	m_cDiskGeometry;

	CAbstractLog*	m_pLog;
	CAbstractFile*	m_pFile;
};

//
// end of class
//

#endif // DISK_HEADER

//
// end of header
//
