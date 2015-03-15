//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface to access hard disk.
//

#ifndef ABSTRACT_DISK_HEADER
#define ABSTRACT_DISK_HEADER

//
// headers
//
#include "Support.h"

//
// class
//
class CAbstractDisk
{
public:
	virtual			~CAbstractDisk() {}

	// Open a disk with disk number.
	virtual int		OpenDisk(size_t nDisk, DWORD dwFlags) = 0;

	// Open a disk with name.
	virtual int		OpenDisk(LPCTSTR lpszDiskName, DWORD dwFlags) = 0;

	// Open the disk with offset.
	virtual int		OpenDisk(UINT64 ui64Offset, UINT64 ui64Length, size_t nDisk, DWORD dwFlags) = 0;

	// Close the disk.
	virtual int		CloseDisk() = 0;

	// Read the buffer from disk.
	virtual int		ReadDisk(BYTE* pBuffer, DWORD dwSectorsToRead, DWORD& dwSectorsRead) = 0;

	// Read the buffer on specified positition from disk.
	virtual int		ReadDisk(BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToRead, DWORD& dwSectorsRead) = 0;

	// Write the buffer to disk on current positition.
	virtual int		WriteDisk(const BYTE* pBuffer, DWORD dwSectorsToWrite, DWORD& dwSectorsWritten) = 0;

	// Write the buffer to specified positition.
	virtual int		WriteDisk(const BYTE* pBuffer, UINT64 ui64Sector, DWORD dwSectorsToWritten, DWORD& dwSectorsWritten) = 0;

	// Get disk position.
	virtual int		GetDiskPointer(INT64& i64DiskPos) = 0;

	// Set disk positition.
	virtual int		SetDiskPointer(INT64 i64DiskPos, INT64& i64NewPos) = 0;

	// Flush the buffer.
	virtual int		FlushDisk() = 0;

	// Lock disk.
	virtual int		LockDisk() = 0;
	virtual int		LockDisk(size_t nDisk) = 0;

	// Unlock the disk.
	virtual int		UnlockDisk() = 0;
	virtual int		UnlockDisk(size_t nDisk) = 0;

	// Check if disk is locked.
	virtual BOOL	IsDiskLock() const = 0;

	// Check for disk existance.
	virtual BOOL	IsDiskExists() const = 0;

	// Check for disk with 8G limit.
	virtual BOOL	Is8GDisk() const = 0;

	// Get start sector of disk.
	virtual BOOL	GetStartSector(INT64* p64StartSector) const = 0;

	// Get total sectors.
	virtual BOOL	GetTotalSectors(INT64* p64Sectors) const = 0;

	// Get sector size.
	virtual BOOL	GetSectorSize(size_t* pBytesPerSector) const = 0;

	// Get disk number.
	virtual BOOL	GetDiskNumber(int* pDiskNumber) const = 0;

	// Get disk geometry.
	virtual BOOL	GetDiskGeometry(INT64* p64Cylinders, size_t* pHeads, INT64* p64Sectors) const = 0;
	virtual BOOL	GetDiskGeometry(DISK_GEOMETRY& cDiskGeometry) const = 0;

	// Get/Set sectors after geometry.
	virtual BOOL	GetSectorsAfterGeometry(size_t& nSectors) const = 0;
	virtual BOOL	SetSectorsAfterGeometry(size_t nSectors) = 0;

	// Get disk model, vendor, and path.
	virtual BOOL	GetDiskInfo(size_t nDisk, TCHAR* pModel, size_t nModelBufferLength,
								TCHAR* pVendor, size_t nVendorBufferLength,
								TCHAR* pPath, size_t nPathBufferLength) const = 0;
};

//
// end of class
//

#endif // ABSTRACT_DISK_HEADER

//
// End of file.
//
