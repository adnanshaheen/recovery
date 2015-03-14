//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface to enter DiskBoard.dll.
//

#ifndef ABSTRACT_DISK_BOARD_INTERFACE_HEADER
#define ABSTRACT_DISK_BOARD_INTERFACE_HEADER

//
// forward decls
//
class CAbstractLog;
class CAbstractDisk;

//
// class
//
class CAbstractDiskBoardInterface
{
public:
	virtual						~CAbstractDiskBoardInterface() {};

	// Create/Delete CAbstractDisk
	virtual CAbstractDisk*		CreateDisk(CAbstractLog* pLog) const = 0;
	virtual void				DeleteDisk(CAbstractDisk* pDisk) = 0;
};

//
// end of class
//

#endif // ABSTRACT_DISK_BOARD_INTERFACE_HEADER

//
// End of file.
//
