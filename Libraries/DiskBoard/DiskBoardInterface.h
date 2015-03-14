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
	virtual CAbstractDisk*		CreateDisk(CAbstractLog* pLog) const;
	virtual void				DeleteDisk(CAbstractDisk* pDisk);

};

//
// end of class.
//

#endif // DISK_BOARD_INTERFACE_HEADER

//
// End of Header file.
//
