//
// Project : LaserRecovery
//
// Author : Adnan Shaheen
//
//
// Purpose : Do not submit any code before its been tested from here.
//

#include "Support.h"
#include "CoreString.h"
#include "DateTime.h"

#include <iostream>

#include "AbstractDisk.h"
#include "AbstractPartInfo.h"
#include "AbstractFileSystem.h"
#include "AbstractPartitioner.h"
#include "AbstractDiskBoardInterface.h"

CAbstractLog* m_pLog = NULL;
CAbstractPartitioner* m_pPartitioner = NULL;
CAbstractDiskBoardInterface* pDiskInterface = NULL;
CAbstractPartInfo* m_pSelectedPartInfo = NULL;

void GetPartition(CAbstractPartitioner* pPartitioner)
{
	size_t nCounter;
	printf("Enter partition number: ");
	scanf("%d", &nCounter);
	size_t nThisPart = 0;
	if (pPartitioner) {

		CAbstractPartInfo* pDiskInfo = pPartitioner->GetDiskItem();
		while (pDiskInfo) {

			if (pDiskInfo->IsFlagExists(MG_PARTINFO_DISK)) {

				CAbstractPartInfo* pPartInfo = pDiskInfo->GetChild();
				while (pPartInfo) {

					if (nCounter == ++nThisPart) {
						m_pSelectedPartInfo = pPartInfo;
						return;
					}
					pPartInfo = pPartInfo->GetNext();
				}
			}
			printf("\n");
			pDiskInfo = pDiskInfo->GetNext();
		}
	}
}

void PrintDisks(CAbstractPartitioner* pPartitioner)
{
	size_t nCounter = 0;
	if (pPartitioner) {

		CAbstractPartInfo* pDiskInfo = pPartitioner->GetDiskItem();
		while (pDiskInfo) {

			if (pDiskInfo->IsFlagExists(MG_PARTINFO_DISK)) {

				printf("Disk %d\n", pDiskInfo->GetDiskNumber());
				CAbstractPartInfo* pPartInfo = pDiskInfo->GetChild();
				while (pPartInfo) {

					printf("\t%d: Partition Type 0x%x\n", ++ nCounter, pPartInfo->GetPartitionType());

					pPartInfo = pPartInfo->GetNext();
				}
			}
			printf("\n");
			pDiskInfo = pDiskInfo->GetNext();
		}
	}
}

int main(char* argv[], int argc)
{
	CDateTime cDateTime;
	cDateTime.GetSystemTime();

	WORD wYear = 0;
	WORD wMonth = 0;
	WORD wDay = 0;
	cDateTime.GetDate(wYear, wMonth, wDay);
	CCoreString csCurrentDateTime;
	csCurrentDateTime.Format(_T("System Current Date is %d-%d-%d\n"), wYear, wMonth, wDay);

	WORD wHour = 0;
	WORD wMinute = 0;
	WORD wSecond = 0;
	cDateTime.GetTime(wHour, wMinute, wSecond);
	csCurrentDateTime.Format(_T("System Current Time is %d:%d:%d\n"), wHour, wMinute, wSecond);

	CCoreString csLibrary;
#ifdef DEBUG
	csLibrary = _T("../../lib/Debug/DiskBoard.dll");
#else
	csLibrary = _T("../../lib/Release/DiskBoard.dll");
#endif // DEBUG
	HMODULE hModule = LoadLibrary(csLibrary);
	if (hModule == NULL || hModule == INVALID_HANDLE_VALUE)
	{
		// Library could not be loaded
		DWORD dwError = GetLastError();
		CCoreString csError;
		csError.Format(_T("Error %d occured while opening library DiskBoard.dll"), dwError);
		ASSERT(FALSE);
		return -1;
	}

	pCreateDiskBoardInterface pCreateDiskInterface =
		(pCreateDiskBoardInterface) GetProcAddress(hModule, "CreateDiskBoardInterface");

	pDiskInterface = pCreateDiskInterface();
	if (pDiskInterface) {
		m_pLog = pDiskInterface->CreateLogFile();
		m_pPartitioner = pDiskInterface->CreatePartitioner(m_pLog);

		/* start process here */
		m_pPartitioner->Initialize();
		PrintDisks(m_pPartitioner);
		GetPartition(m_pPartitioner);

		/* all disks are detected, file systems on their partitions are also detected */
		/* TODO: we have to select a partition with ntfs file system */
		/* read the ntfs deleted files and recover them */
		/* create a CFileSystem class, and inherit it to all other CNTFSFileSystem etc */
		CAbstractFileSystem* pFileSystem = pDiskInterface->CreateFileSystem(m_pLog);
		if (pFileSystem) {
			if (pFileSystem->Initialize(m_pSelectedPartInfo)) {
				/* we have succussfully initialized File System, now start scan process */
				pFileSystem->Scan();
			}
		}

		pDiskInterface->DeleteFileSystem(pFileSystem);
		pDiskInterface->DeletePartitioner(m_pPartitioner);
		pDiskInterface->DeleteLogFile(m_pLog);
	}

	pDeleteDiskBoardInterface pDeleteDiskInterface =
		(pDeleteDiskBoardInterface) GetProcAddress(hModule, "DeleteDiskBoardInterface");
	if (pDeleteDiskInterface) {
		pDeleteDiskInterface(pDiskInterface);
	}

	FreeLibrary(hModule);

	return 0;
}

// End of test.
//
