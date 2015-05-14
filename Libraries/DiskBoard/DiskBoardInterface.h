//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for Disk Board DLL.
//

#ifndef DISK_BOARD_INTERFACE_HEADER
#define DISK_BOARD_INTERFACE_HEADER

//
// delcs
//
class CAbstractLog;

//
// headers
//
#include "AbstractDiskBoardInterface.h"

//
// class
//
class CDiskBoardInterface : public CAbstractDiskBoardInterface
{
public:
	CDiskBoardInterface();
	virtual ~CDiskBoardInterface();

	// Create/Delete CDisk
	virtual CAbstractDisk*			CreateDisk(CAbstractLog* pLog) const;
	virtual void					DeleteDisk(CAbstractDisk* pDisk);

	// Create/Delete Partitioner
	virtual CAbstractPartitioner*	CreatePartitioner(CAbstractLog* pLog) const;
	virtual void					DeletePartitioner(CAbstractPartitioner* pPartitioner);

	// Create/Delete Partition Info
	virtual CAbstractPartInfo*		CreatePartInfo(CAbstractLog* pLog) const;
	virtual void					DeletePartInfo(CAbstractPartInfo* pPartInfo);

	// Create/Delete log
	virtual CAbstractLog*			CreateLogFile() const;
	virtual void					DeleteLogFile(CAbstractLog* pLog);

	/**
	 * CreateFileSystem
	 * Create file system interface
	 */
	virtual CAbstractFileSystem*	CreateFileSystem(CAbstractLog* pLog) const;
	/**
	 * DeleteFileSystem
	 * Delete file system interface
	 */
	virtual void					DeleteFileSystem(CAbstractFileSystem* pFileSystem);
};

//
// end of class.
//

#endif // DISK_BOARD_INTERFACE_HEADER

//
// End of Header file.
//
