
/*
 * PartitionTable.h
 *
 * Data structures for direct access to PC partitions and partition infos.
 */

#pragma once

#include "Support.h"

#pragma pack(1)

class CSectorCylinder
{
	BYTE m_hiCylAndSec;
	BYTE m_loCylinder;

public:
	CSectorCylinder() { }
	CSectorCylinder(int nSector, int nCylinder)
	{
		ASSERT(nSector >= 1 && nSector <= 0x3f);		// Boundaries given by the
		ASSERT(nCylinder >= 0 && nCylinder <= 0x3ff );	// Representation in two bytes

		m_loCylinder = (BYTE) nCylinder;
		m_hiCylAndSec = (nSector & 0x3F) + (nCylinder & 0x300) / 4;
	}

	int	Cylinder() const
	{
		return m_loCylinder + (((int) m_hiCylAndSec) & 0xC0) * 4;
	}
	int Sector() const
	{
		return m_hiCylAndSec & 0x3F;
	}
};

// class CHeadSectorCylinder
class CHeadSectorCylinder
{
private:
	BYTE m_cData[3];
	void init(int nHead, int nSector, int nCylinder)
	{
		m_cData[0] = (BYTE) nHead;
		CSectorCylinder tmp(nSector, nCylinder);
		memcpy (m_cData + 1, &tmp, 2);
	}

public:
	CHeadSectorCylinder() { }
	CHeadSectorCylinder(int nHead, int nSector, int nCylinder)
	{
		init(nHead, nSector, nCylinder);
	}

	// Construct from LBA and given disc geometry
	CHeadSectorCylinder(long LBA, const CHeadSectorCylinder &geom)
	{
		init (
				(LBA % (geom.Head ()*geom.Sector ())) / geom.Sector (),		// Head
				(LBA % (geom.Head ()*geom.Sector ())) % geom.Sector () + 1,	// Sector
				 LBA / (geom.Head ()*geom.Sector ())						// Cylinder
		);
	}

	// Read access the internal values
	int Head() const
	{
		return m_cData[0];
	}
	int Sector() const
	{
		return ((CSectorCylinder*) &m_cData[1])->Sector();
	}
	int Cylinder() const
	{
		return ((CSectorCylinder*) &m_cData[1])->Cylinder();
	}
	CSectorCylinder	SectorCylinder() const
	{
		return *((CSectorCylinder*)& m_cData[1]);
	}

	long LBA(const CHeadSectorCylinder &geom) const
	{
		return ((Cylinder() * geom.Head() + Head()) * geom.Sector()) + Sector() - 1;
	}

	CHeadSectorCylinder& operator = (const CHeadSectorCylinder& r)
	{
		// Assure that unless 4==sizeof *this only the 3 relevant bytes are copied:
		memcpy(this, &r, 3);
		return *this;
	}

	BOOL operator == (const CHeadSectorCylinder& CHS)
	{
		return (memcmp(this, &CHS, 3) == 0) ? TRUE : FALSE;
	}

	BOOL operator < (const CHeadSectorCylinder& CHS)
	{
		if (Cylinder() < CHS.Cylinder())
			return TRUE;
		if (Cylinder() > CHS.Cylinder())
			return FALSE;

		if (Head() < CHS.Head())
			return TRUE;
		if (Head() > CHS.Head())
			return FALSE;

		return (Sector() < CHS.Sector());
	}

	BOOL operator > (const CHeadSectorCylinder& CHS)
	{
		if (Cylinder() > CHS.Cylinder())
			return TRUE;
		if (Cylinder() < CHS.Cylinder())
			return FALSE;

		if (Head() > CHS.Head())
			return TRUE;
		if (Head() < CHS.Head())
			return FALSE;

		return (Sector() > CHS.Sector());
	}
};

// A partition entry as stored inside a root sector on HD:
class CPartitionEntry
{
	BYTE	m_data[16];
public:
	// Access methods:
	BYTE					Status() const				{ return m_data[0];									}
	BYTE&					Status()					{ return m_data[0];									}
	CHeadSectorCylinder		Start() const				{ return *(CHeadSectorCylinder*) &m_data[1];		}
	CHeadSectorCylinder&	Start()						{ return *(CHeadSectorCylinder*) &m_data[1];		}
	BYTE					PartitionType() const		{ return m_data[4];									}
	BYTE&					PartitionType()				{ return m_data[4];									}
	CHeadSectorCylinder		End() const					{ return *(CHeadSectorCylinder*) &m_data[5];		}
	CHeadSectorCylinder&	End()						{ return *(CHeadSectorCylinder*) &m_data[5];		}
	DWORD					DistOfFirstSector() const	{ return *(DWORD*) &m_data[8];						}
	DWORD&					DistOfFirstSector()			{ return *(DWORD*) &m_data[8];						}
	DWORD					NumSectors() const			{ return *(DWORD*) &m_data[12];						}
	DWORD&					NumSectors()				{ return *(DWORD*) &m_data[12];						}
	BOOL					IsValid() const				{ return (PartitionType() != 0);					}
	BOOL					IsGPT() const				{ return PartitionType() == 0xEE ? TRUE : FALSE;	}

	BOOL operator == (const CPartitionEntry& Entry)
	{
		return (Start() == Entry.Start());
	}

	BOOL operator < (const CPartitionEntry& Entry)
	{
		return (Start() < Entry.Start());
	}

	BOOL operator > (const CPartitionEntry& Entry)
	{
		return (Start() > Entry.Start());
	}

	BOOL operator <= (const CPartitionEntry& Entry)
	{
		return (((*this) < Entry) || ((*this) == Entry));
	}

	BOOL operator >= (const CPartitionEntry& Entry)
	{
		return (((*this) > Entry) || ((*this) == Entry));
	}
};

// Root sector as read from hard disk:
class CRootSector
{
public:
	// Construction/Destruction:
	CRootSector()
	{
		BYTE BootCode[] =
		{
			/* 0x00000000 */	0xFA, 0xBC, 0x00, 0x7C, 0x29, 0xC0, 0x8E, 0xD0, 0xFB, 0xBE, 0x20, 0x7C, 0x50, 0x1F, 0xBF, 0x20, 	// ��.|)����� |P.� 
			/* 0x00000010 */	0x06, 0x50, 0x07, 0xB9, 0xF0, 0x00, 0xFC, 0xF3, 0xA5, 0xEA, 0x20, 0x06, 0x00, 0x00, 0x90, 0x90, 	// .P.��.��� ...��
			/* 0x00000020 */	0xBE, 0xBE, 0x07, 0x80, 0x3C, 0x80, 0x74, 0x54, 0xCD, 0x18, 0xBE, 0x40, 0x06, 0xAC, 0x08, 0xC0, 	// ��.�<�tT�.�@.�.�
			/* 0x00000030 */	0x75, 0x02, 0xEB, 0xFE, 0x56, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0x5E, 0xEB, 0xEE, 0x90, 	// u.��V�..�.�.^��
			/* 0x00000040 */	0x45, 0x72, 0x72, 0x6F, 0x72, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 	// Error loading op
			/* 0x00000050 */	0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x4D, 	// erating system.M
			/* 0x00000060 */	0x69, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 	// issing operating
			/* 0x00000070 */	0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x90, 0x90, 0x90, 0x90, 0xAD, 0x89, 0xC2, 0xAD, 	//  system.������­
			/* 0x00000080 */	0x89, 0xC1, 0xBD, 0x06, 0x00, 0x55, 0xB8, 0x01, 0x02, 0xBB, 0x00, 0x7C, 0xCD, 0x13, 0x5D, 0x73, 	// ���..U�..�.|�.]s
			/* 0x00000090 */	0x0F, 0x29, 0xC0, 0x52, 0x55, 0x28, 0xD2, 0xCD, 0x13, 0x5D, 0x5A, 0x4D, 0x75, 0xE7, 0xEB, 0x8A, 	// .)�RU(��.]ZMu��
			/* 0x000000A0 */	0xBE, 0x5F, 0x06, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x75, 0x82, 0xBE, 0xE6, 0x06, 0x29, 0xC0, 	// �_.�>�}U�u���.)�
			/* 0x000000B0 */	0x29, 0xED, 0xB9, 0x10, 0x00, 0xAD, 0x01, 0xC5, 0x49, 0x75, 0xFA, 0xBE, 0x5F, 0x06, 0x81, 0xFD, 	// )��..�.�Iu��_.��
			/* 0x000000C0 */	0x2B, 0xC0, 0x75, 0xE5, 0xBE, 0xE8, 0x06, 0x80, 0x7C, 0x1F, 0xAA, 0x74, 0x03, 0xBE, 0x08, 0x07, 	// +�u��.�|.�t.�..
			/* 0x000000D0 */	0xAC, 0x08, 0xC0, 0x74, 0x0B, 0x56, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0x5E, 0xEB, 0xF0, 	// �.�t.V�..�.�.^��
			/* 0x000000E0 */	0xBE, 0xBE, 0x07, 0xEA, 0x00, 0x7C, 0x00, 0x00, 0x4E, 0x65, 0x72, 0x6F, 0x20, 0x42, 0x6F, 0x6F, 	// ��.�.|..Nero Boo
			/* 0x000000F0 */	0x74, 0x2D, 0x4C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x20, 0x56, 0x33, 0x2E, 0x30, 0x0D, 0x0A, 0x0A, 	// t-Loader V3.0...
			/* 0x00000100 */	0x00, 0x61, 0x68, 0x65, 0x61, 0x64, 0x21, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// .ahead!�........
			/* 0x00000110 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000120 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000130 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000140 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000150 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000160 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000170 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000180 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000190 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x000001A0 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x000001B0 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		memset(this, 0, sizeof(*this));							// Wipe entire structure
		memcpy(m_cBootCode, BootCode, sizeof(m_cBootCode));		// Copy code template
		ASSERT(sizeof(m_cBootCode) == sizeof(BootCode));		// Assume code template has correct size!
		m_cMagicAA55 = 0xAA55;									// Set magic value
	}

	~CRootSector()
	{
		// Assume this structure has correct size
		ASSERT(sizeof *this == 512);
	}

	int PartEntries(void) const
	{
		return (sizeof(m_cPartEntry) / sizeof(CPartitionEntry));	// 4
	}

	const CPartitionEntry& operator[](int iIndex) const
	{
		if((iIndex < 0) || (iIndex >= PartEntries()))
		{
			ASSERT(FALSE);
		}
		return m_cPartEntry[iIndex];
	}

	CPartitionEntry& operator[](int iIndex)
	{
		if((iIndex < 0) || (iIndex >= PartEntries()))
		{
			ASSERT(FALSE);
		}
		return m_cPartEntry[iIndex];
	}

	BOOL IsValid(void) const
	{
		return (m_cMagicAA55 == 0xAA55);
	}

private:
	// Member variables:
	BYTE				m_cBootCode[446];		// ix86 Partition boot code
	CPartitionEntry		m_cPartEntry[4];		// Maximum of 4 partition entries
	WORD				m_cMagicAA55;			// Magic value to mark a correct root sector
};

class CGPTHeader
{
public:
	CGPTHeader()
	{
		m_cMagic = 0;
		m_nRevision = 0;
		m_nHeaderSize = 0;
		m_nCRC32_Header = 0;
		m_nReserved = 0;
		m_i64LBA = 0;
		m_i64BackupLBA = 0;
		m_i64FirstLBA = 0;
		m_i64LastLBA = 0;
		ZeroMemory(m_DiskGUID, sizeof(m_DiskGUID));
		m_i64StartingLBA = 0;
		m_nNumber = 0;
		m_nSize = 0;
		m_nCRC32_Table = 0;
	}

	~CGPTHeader()
	{
	}

	BOOL IsValid() const
	{
		return m_cMagic == 0x5452415020494645ULL;
	}

	s64 GetFirstEntry() const
	{
		return m_i64FirstLBA;
	}

	s32 GetSize() const
	{
		return m_nSize;
	}

private:
	s64 m_cMagic;		  /* Signature ("EFI PART", 45h 46h 49h 20h 50h 41h 52h 54h or 0x5452415020494645ULL */
    s32 m_nRevision;	  /* (for GPT version 1.0 (through at least UEFI version 2.3.1), the value is 00h 00h 01h 00h) */
    s32 m_nHeaderSize;    /* size in little endian (in bytes, usually 5Ch 00h 00h 00h meaning 92 bytes) */
    s32 m_nCRC32_Header;  /* of header (offset +0 up to header size), with this field zeroed during calculation */
    s32 m_nReserved;      /* must be zero */
    s64 m_i64LBA;         /* LBA (location of this header copy) */
    s64 m_i64BackupLBA;   /* LBA (location of the other header copy) */
    s64 m_i64FirstLBA;    /* usable LBA for partitions (primary partition table last LBA + 1) */
    s64 m_i64LastLBA;     /* usable LBA (secondary partition table first LBA - 1) */
    u8  m_DiskGUID[16];   /* also referred as UUID on UNIXes */
    s64 m_i64StartingLBA; /* of array of partition entries (always 2 in primary copy) */
    s32 m_nNumber;        /* of partition entries in array */
    s32 m_nSize;          /* of a single partition entry (usually 128) */
    s32 m_nCRC32_Table;   /* of partition array */
};

class CGPTPartitions
{
public:
	CGPTPartitions()
	{
		ZeroMemory(m_PartTypeGUID, sizeof(m_PartTypeGUID));
		ZeroMemory(m_PartGUID, sizeof(m_PartGUID));
		m_i64FirstLBA = 0;
		m_i64LastLBA = 0;
		m_i64AttrFlags = 0;
		ZeroMemory(m_cPartName, sizeof(m_cPartName));
	}

	~CGPTPartitions()
	{
	}

private:
	u8 m_PartTypeGUID[16]; /* partition type GUID */
	u8 m_PartGUID[16];     /* partition GUID */
	s64 m_i64FirstLBA;     /* start of entry */
	s64 m_i64LastLBA;      /* end of entry */
	s64 m_i64AttrFlags;    /* attribute flags (e.g. bit 60 denotes read-only) */
	u16 m_cPartName[36];   /* entry name (36 UTF-16LE code units) */
};

#pragma pack()

//
// End of PartitionInfo.h
//

