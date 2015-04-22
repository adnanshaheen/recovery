//
// Project : 
//
// Author : Adnan Shaheen
//
//
// Purpose : combined data types.
//

#ifndef TYPES_HEADER
#define TYPES_HEADER

//
// types
//

#include <string>
#include <assert.h>
#include <windows.h>
#include <tchar.h>

#undef DEBUG
#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#include "Bits.h"

/**
 * consts/defines
 */
const TCHAR kszApp[]	= _T("recovery");
#define MAX_HDD_SUPPORTED 50

#ifdef WIN32
//
// WIN32 data types
//
#define MG_DECL						__cdecl

#define MG_INVALID_FILE				((DWORD)-1)
#define PATH_SEPARATOR	_T('\\')

#else // WIN32
//
// LINUX/UNIX data types
//
#define MG_DECL

#define MG_INVALID_FILE		-1
#define PATH_SEPARATOR	_T('/')

typedef enum eMediaType
{
	HD_Unknown,						// Format is unknown.
	HD_F5_1Pt2_512,					// 5.25", 1.2MB,  512 bytes/sector.
	HD_F3_1Pt44_512,				// 3.5",  1.44MB, 512 bytes/sector.
	HD_F3_2Pt88_512,				// 3.5",  2.88MB, 512 bytes/sector.
	HD_F3_20Pt8_512,				// 3.5",  20.8MB, 512 bytes/sector.
	HD_F3_720_512,					// 3.5",  720KB,  512 bytes/sector.
	HD_F5_360_512,					// 5.25", 360KB,  512 bytes/sector.
	HD_F5_320_512,					// 5.25", 320KB,  512 bytes/sector.
	HD_F5_320_1024,					// 5.25", 320KB,  1024 bytes/sector.
	HD_F5_180_512,					// 5.25", 180KB,  512 bytes/sector.
	HD_F5_160_512,					// 5.25", 160KB,  512 bytes/sector.
	HD_RemovableMedia,				// Removable media other than floppy.
	HD_FixedMedia,					// Fixed hard disk media.
	HD_F3_120M_512,					// 3.5", 120M Floppy.
	HD_F3_640_512,					// 3.5" ,  640KB,  512 bytes/sector.
	HD_F5_640_512,					// 5.25",  640KB,  512 bytes/sector.
	HD_F5_720_512,					// 5.25",  720KB,  512 bytes/sector.
	HD_F3_1Pt2_512,					// 3.5" ,  1.2Mb,  512 bytes/sector.
	HD_F3_1Pt23_1024,				// 3.5" ,  1.23Mb, 1024 bytes/sector.
	HD_F5_1Pt23_1024,				// 5.25",  1.23MB, 1024 bytes/sector.
	HD_F3_128Mb_512,				// 3.5" MO 128Mb   512 bytes/sector.
	HD_F3_230Mb_512,				// 3.5" MO 230Mb   512 bytes/sector.
	HD_F8_256_128,					// 8",     256KB,  128 bytes/sector.
	HD_F3_200Mb_512,				// 3.5",   200M Floppy (HiFD).
	HD_F3_240M_512,					// 3.5",   240Mb Floppy (HiFD).
	HD_F3_32M_512					// 3.5",   32Mb Floppy.
} MG_MEDIA_TYPE;

typedef union _LARGE_INTEGER		// LARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	};

	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;

	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER		// ULARGE_INTEGER
{
  struct
  {
		DWORD LowPart;
		DWORD HighPart;
	};

	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;

	ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

typedef struct cDiskGeometry
{
	cDiskGeometry()
	{
		nCylinders.LowPart	= 0;
		nCylinders.HighPart	= 0;
		cMediaType			= HD_Unknown;
		dwTracksPerCylinder	= 0;
		dwSectorsPerTrack	= 0;
		dwBytesPerSector	= 0;
		dwHeads				= 0;
	}

    LARGE_INTEGER			nCylinders;
    LR_MEDIA_TYPE			cMediaType;
    DWORD					dwTracksPerCylinder;
    DWORD					dwSectorsPerTrack;
    DWORD					dwBytesPerSector;
	DWORD					dwHeads;
} MG_DISK_GEOMETRY, *PMG_DISK_GEOMETRY;


typedef struct cDiskGeometryEx
{
	MG_DISK_GEOMETRY		cGeometry;
	LARGE_INTEGER			nDiskSize;
	BYTE					cData[1];
} MG_DISK_GEOMETRYEX, *PMG_DISK_GEOMETRYEX;

#endif // WIN32

//
// COMMON TYPES
//
typedef enum eIconType
{
	MG_ICON_INFO = 0,
	MG_ICON_ERROR,
	MG_ICON_WARNING,
	MG_ICON_QUESTION,
	MG_ICON_DRIVE,
	MG_ICON_FILE,
	MG_ICON_UNKNOWN
} MG_ICON_TYPE;

typedef enum eFileMove
{
	MG_FILE_POS_BEGIN	= 0,
	MG_FILE_POS_CURRENT = 1,
	MG_FILE_POS_END		= 2
} MG_FILE_MOVE;

typedef enum eFileResult
{
	MG_END_OF_LINE = -2,
} MG_FILE_RESULT_TYPE;

typedef enum eWorkType
{
} MG_WORK_TYPE;

typedef enum {
	_E_REP_PARAM_ERROR   = -7,
	_E_REP_DRIVE_ERROR   = -6,
	_E_REP_READ_FAIL     = -5,
	_E_REP_DEVICEIO_FAIL = -4,
	_E_REP_MEMORY_FAIL   = -3,
	_E_REP_GEOMTRY_FAIL  = -2,
	_E_REP_HANDLE_FAIL   = -1,
	_E_REP_SUCCESS       = 0,
};

typedef enum {
	MG_PARTINFO_DISK	= 0x00000001,
	MG_PARTINFO_GUID	= 0x00000002,
	MG_PARTINFO_EFI		= 0x00000004,
};

typedef enum {
	MG_PARTTYPE_HFSPLUS	= 0xAF,
};

//
// COMMON CONST
//

#ifndef MG_FILE_POINTER_INVALID
#define MG_FILE_POINTER_INVALID		MG_INVALID_FILE
#endif // MG_FILE_POINTER_INVALID

#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE			65536
#endif // FILE_BUFFER_SIZE

#ifndef FORCE_ANSI
#define FORCE_ANSI					0x10000
#endif // FORCE_ANSI

#ifndef FORCE_UNICODE
#define FORCE_UNICODE				0x20000
#endif // FORCE_UNICODE

#ifndef FORCE_INT64
#define FORCE_INT64					0x40000
#endif // FORCE_INT64

#ifndef MAX_DRIVE_SUPPORT
#define MAX_DRIVE_SUPPORT			50
#endif // MAX_DRIVE_SUPPORT

//
// COMMON MACROS/METHODS
//

#ifdef _UNICODE
#define WIDEN2(x)	L ## x
#define WIDEN(x)	WIDEN2(x)
#define __TFILE__	WIDEN(__FILE__)
#else
#define __TFILE__	__FILE__
#endif // _UNICODE

#define MGMax(a,b)					(a<b?b:a)
#define MGMin(a,b)					(a>b?b:a)

#define GET_LENGTH(a)				(a?(int)strlen(a):0)
#define GET_WLENGTH(a)				(a?(int)wcslen(a):0)
#define GET_TLENGTH(a)				(a?(int)_tcslen(a):0)

#define strinc(a)					++ a
#define STRING_INC_CHAR(a)			strinc(a)

#define STRUPR(a)					for(size_t i=0;i<_tcslen(a);++i)\
									{ a[i]=toupper(a[i]); }
#define STRLWR(a)					for(size_t i=0;i<_tcslen(a);++i)\
									{ a[i]=tolower(a[i]); }

inline void ASSERT_MACRO(BOOL bResult, LPCSTR pszFileName, int nLineNo)
{
	if (!bResult)
	{
		LPTSTR pFileName = (LPTSTR) strrchr(pszFileName, '/');
		if (pFileName != NULL)
		{
			++ pFileName;
			printf("Assert Failed! File %s, Line %d\n", pFileName, nLineNo);
		}
		else
		{
			printf("Assert Failed!\n");
		}
	}
}

#ifdef ASSERT
#undef ASSERT
#endif // ASSERT

#ifndef ASSERT
#define ASSERT(a)	ASSERT_MACRO((BOOL)a,__FILE__,__LINE__)
#endif // ASSERT

#ifdef assert
#undef assert
#endif // assert

#ifndef assert
#define assert		ASSERT
#endif // assert

/**
 * macros
 */
#define TEST_AND_THROW(a,b)		if (a) throw b;
#define TEST_AND_RETURN(a,b)	if (a) return b;
#define DELETEME(a)				if (a) delete a, a = NULL;
#define DELETE_ARRAY(a)			if (a) delete[] a, a = NULL;

#endif // TYPES_HEADER

//
// End of file.
//
