//
// Project : Disk Board
//
// Author : Adnan Shaheen
//
//
// Purpose : Disk Board DLL interface implementation.
//

#include "Support.h"
#include "DiskBoard.h"
#include "DiskBoardInterface.h"

//
// Dll entry
//
#ifdef WIN32
BOOL APIENTRY DllMain(	HANDLE hModule, 
						DWORD  dwReasonForCall, 
						VOID* pReserved
					 )
{
    switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}
#endif // WIN32

//
//  Create new Disk Board Interface
//
extern "C" MG_DISK_BOARD_DLL CAbstractDiskBoardInterface* CreateDiskBoardInterface()
{
	return new CDiskBoardInterface();
}

//
// Delete Disk Board Interface
//
extern "C" MG_DISK_BOARD_DLL void DeleteDiskBoardInterface(CAbstractDiskBoardInterface* pInterface)
{
	if (pInterface != NULL)
		delete pInterface, pInterface = NULL;
}

//
// End of Source file.
//
