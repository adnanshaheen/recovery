//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Disk Board DLL interface.
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
// decls
//
class CAbstractDiskBoardInterface;

//
// Create new Disk Board Interface.
//
extern "C" MG_DISK_BOARD_DLL CAbstractDiskBoardInterface* CreateDiskBoardInterface();
typedef CAbstractDiskBoardInterface* (MG_DECL pCreateDiskBoardInterface)();

//
// Delete Disk Board Interface.
//
extern "C" MG_DISK_BOARD_DLL void DeleteDiskBoardInterface(CAbstractDiskBoardInterface* pInterface);
typedef CAbstractDiskBoardInterface* (MG_DECL pDeleteDiskBoardInterface)(CAbstractDiskBoardInterface* pInterface);

//
// End of header.
//
