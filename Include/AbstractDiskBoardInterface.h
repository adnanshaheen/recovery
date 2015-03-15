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
class CAbstractPartitioner;
class CAbstractPartInfo;

//
// class
//
class CAbstractDiskBoardInterface
{
public:
	virtual							~CAbstractDiskBoardInterface() {};

	// Create/Delete CAbstractDisk
	virtual CAbstractDisk*			CreateDisk(CAbstractLog* pLog) const = 0;
	virtual void					DeleteDisk(CAbstractDisk* pDisk) = 0;

	// Create/Delete Partitioner
	virtual CAbstractPartitioner*	CreatePartitioner(CAbstractLog* pLog) const = 0;
	virtual void					DeletePartitioner(CAbstractPartitioner* pPartitioner) = 0;

	// Create/Delete Partition Info
	virtual CAbstractPartInfo*		CreatePartInfo(CAbstractLog* pLog) const = 0;
	virtual void					DeletePartInfo(CAbstractPartInfo* pPartInfo) = 0;

	// Create/Delete log
	virtual CAbstractLog*			CreateLogFile() const = 0;
	virtual void					DeleteLogFile(CAbstractLog* pLog) = 0;
};

//
// end of class
//

#ifdef	WIN32
#	ifdef	MG_DISK_BOARD
#		define	MG_DISK_BOARD_DLL		__declspec(dllexport)
#	else	// MG_DISK_BOARD
#		define	MG_DISK_BOARD_DLL		__declspec(dllimport)
#	endif	// MG_DISK_BOARD
#else	// WIN32
#	define	MG_DISK_BOARD_DLL
#endif	// WIN32

//
// Create new Disk Board Interface.
//
extern "C" MG_DISK_BOARD_DLL CAbstractDiskBoardInterface* CreateDiskBoardInterface();
typedef CAbstractDiskBoardInterface* (MG_DECL *pCreateDiskBoardInterface)();

//
// Delete Disk Board Interface.
//
extern "C" MG_DISK_BOARD_DLL void DeleteDiskBoardInterface(CAbstractDiskBoardInterface* pInterface);
typedef CAbstractDiskBoardInterface* (MG_DECL *pDeleteDiskBoardInterface)(CAbstractDiskBoardInterface* pInterface);

#endif // ABSTRACT_DISK_BOARD_INTERFACE_HEADER

//
// End of file.
//
